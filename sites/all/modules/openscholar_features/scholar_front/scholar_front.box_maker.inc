<?php


/**
 * @file batch functions for creating boxes from existing front pages
 **/

require_once drupal_get_path('module', 'os_boxes') . '/os_boxes.newbox_api.inc';

//completed batch
function front_boxmaker_done($success, $results, $operations) {
  if ($success) {
  	$message = 'Success making boxes.';
  } else {
  	$message = 'Boxmaker failed.  Have fun debugging!';
  }
  
  drupal_set_message($message);
  $_SESSION['my_batch_results'] = 'Boxed frontpages for vsites: ' . implode(', ', $results);
}


//call boxmaker, starting at offset
function _front_boxmaker($limit, $offset, $total_vsites, &$context) {
  global $_box_plugins;
  $_box_plugins  = os_boxes_boxes_plugins();
  $_box_plugins += spaces_simpleviews_boxes_plugins();

  
  //prep sandbox
  if (empty($context['sandbox'])) {
  	$context['sandbox']['progress'] = 0;
  	$context['sandbox']['max'] = $total_vsites;
  	$context['message'] = 'Processing vsites 0 of ' . $total_vsites;
  }
  
  //loop over all users
  $query = 'SELECT uid FROM {users} LIMIT %d OFFSET %d ';
  $result = db_query($query, (int)$limit, (int)$offset);
  while ($res = db_fetch_object($result)) {
    
    if (($vsites = vsite_get_vsite_by_owner($res->uid)) == FALSE) {
      return; //user has no vsites.  ignore.
    }
    
    //loop over this user's vsites
    foreach($vsites as $vsite) {
      //update old box IDs
      update_block_ids($vsite);
      
      // create new frontpage box from old frontpage settings
      $settings = fb_front_settings($vsite->id);
      $front = fb_get_front($settings, $vsite);
      
      if ($front && ($wgt = fb_mk_front_box($vsite, $front)) != FALSE) {
      	$add_func = 'fb_add_' . $wgt['wgt']; //box or block
        $context['results'][] = $add_func($vsite, $wgt['data']);
        $vsite->controllers->variable->del('site_frontpage'); //remove previous front page

        $context['message'] = 'Processing vsites ' . $offset . '-' . min($offset+$limit, $total_vsites) . ' of ' . $total_vsites;
        $context['sandbox']['progress']++;
      } else {
        error_log('Could not determine front page for vsite ' . $vsite->id, 'warning');
      }
    }
  }
}


// get settings for this vsite's front page
// settings are two parts.  there are several types of settings.  list the type.  its settings will vary.
function fb_front_settings($vsite_id) {
  $query  = 'SELECT object_id,value FROM {spaces_overrides} WHERE id = "%s" ';
  $query .= 'AND object_id IN ("site_frontpage", "scholar_front_frontpage_nid", "scholar_front_frontpage_path", "scholar_front_feature_nodes", "schoalr_front_create_bio", "scholar_front_site_title")';
  $result = db_query($query, $vsite_id);
  $settings = array();
  while ($obj = db_fetch_object($result)) {
    $settings[$obj->object_id] = unserialize($obj->value);
  }
  
  return $settings;
}


// given db info on front page, return settings for new box to make.
function fb_get_front($settings, &$vsite) {

  if (!count($settings)) {
  	return false; //no settings, no box.  default box should come from presets, not here.
  }
  
  $front = array();
  if (isset($settings['scholar_front_site_title'])) {
    $front['title'] = $settings['scholar_front_site_title'];
  }
  
  switch ($settings['site_frontpage']) {

  	//features content
    case 'scholar_front/features':
      $front['type'] = 'features';
      break;
      
    //flagged content
    case 'scholar_front/flag':
      $front['type'] = 'flag';
      break;
      
    //path or alias to node
    case $settings['scholar_front_frontpage_path']:
      $aliased_url =  $vsite->group->purl . '/' . $settings['scholar_front_frontpage_path'];
      
   	  if ($node_path = drupal_get_normal_path($aliased_url)) {
   	  	//we have a nid.  this is not a path frontpage at all, but a nid path pointing to an alias
   	  	$front['type'] = 'nid';
   		  $front['options'] = array('node' => $node_path);
   	  } else {
   	  	//no, still a path front
  	    $front['type'] = 'path';
        $front['options'] = array('path' => $settings['scholar_front_frontpage_path']);
   	  }
      break;
    
    //bio.
    case 'create_bio':
      $front['type'] = 'bio';
      break;
  }
  
  //no need to continue if we have a type
  if (isset($front['type'])) {
    return $front;
  }
  
  //no type yet.  frontpage is a node/nid.  maybe bio, maybe nid.
  $nid = (int) end(split('/', $settings['site_frontpage']));
  
  $query = 'SELECT type FROM {node} WHERE nid = %d';
  $result = db_query($query, $nid);
  $r = db_fetch_object($result);
  $type = $r->type;
  
  if ($type == 'bio') {
    $front['type'] = 'bio';
    $front['options'] = array('node' => $settings['site_frontpage']);
  } elseif (isset($type)) {
    $front['type'] = 'nid';
    $front['options'] = array('node' => $settings['site_frontpage']);
  }  else {
    //no node by this nid.  maybe an old setting for a node that was deleted?  no box!
    return false;
  }
  
  return $front;
}

//make a box for each vsite.
function fb_mk_front_box(&$vsite, $settings) {
  
  if (!$settings) {
    return false;
  }

  $opts = array();
  global $_box_plugins;
 
  switch ($settings['type']) {
  	
  	//bio box
    case 'bio':
      $nid = (int) end(explode('/', $settings['options']['node'])); //get nid from node/978897
      
      //check for blurb
	  $query = <<<EOD
SELECT DISTINCT nid,field_hook_text_value
FROM {content_type_bio}
WHERE field_hook_text_value IS NOT NULL
AND   nid = %d
GROUP BY nid HAVING max(vid)
EOD;
      $result = db_query($query, $nid);

      if ($r = db_fetch_object($result)) {
      	//we have a blurb.  let's make a simple box for it.
      	$type = 'os_boxes_simple';
      	$opts['body'] = $r->field_hook_text_value;
      	
      } else {
      	//nope, just a regular bio box
      	$type = 'os_boxes_bio';
      	$opts['nid'] = $nid;
      	$opts['teaser'] = 'full';
      }
      
      break;
      
    //node box
    case 'nid':
      $type = 'os_boxes_node';
      $opts['nid'] = (int) end(explode('/', $settings['options']['node'])); //get nid from node/978897
      $opts['teaser'] = 'full';
      break;
      
      
    //no support for path boxes
    case 'path':
      //path as a front page box is not supported.
      return false;
      
      
    //featuredposts block.  no new box supoport.  note flag to make this a block
    case 'flag':
      $type = 'views-vsite_featuredposts-block_1';
      $block = true;
      break;
      
      
    //features uses simpleviews_boxes_ls
    case 'features':
      $type = 'spaces_simpleviews_post_list';
      $opts['style'] = 'teasers';
      $opts['filter'] = 'all';
      break;
  }
  
  if (!isset($type)) {
    $type = 'unknown';
  }
  
  //set some labels/defaults
  if (isset($settings['title'])) {
    $opts['title'] = $settings['title'];
    $opts['subject'] = $settings['title'];
  } else {
  	$opts['title'] = '';
  	$opts['subject'] = '';
  }
  $opts['description'] = $_box_plugins[$type]['title'] . ' box';
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  //debug log.  also writes vsite link, so we can find a type of box and compare local site vs what's in prod
  //@todo remove this debug log
  global $base_url;
  $msg = $type . "\t\t" . $base_url . '/' . $vsite->group->purl;
  $msg .= "\t\t" . 'http://scholar.harvard.edu/' . $vsite->group->purl . "\n";
  error_log($msg, 3, '/tmp/boxmaker.txt');
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  
  //make and return array.  array specifies if widget is box or block and then includes it as data
  if (!$block) {
  	//return a box
  	if ($box = mk_box($vsite, $type, $opts)) {
  	  return array('wgt' => 'box', 'data' => $box);
  	} else {
  	  return false;
  	}
  } else {
  	//no box for this type.  return a deprecated block
  	return array('wgt' => 'block', 'data' => $type);
  }
}


//add our new box to content_top
function fb_add_box(&$vsite, $box) {
  return vsite_put_box($vsite, $box,'content_top');
}

//adds block to content_top region
function fb_add_block(&$vsite, $block) {
  $front_context = $vsite->controllers->context->get('openscholar-feature-front:reaction:block');
  $front_context['blocks'][$block]['region'] = 'content_top';

  if ($vsite->controllers->context->set('openscholar-feature-front:reaction:block:'.$vsite->group->og_theme, $front_context)) {
    return $vsite->id;
  }
}

// update the ids of all blocks on this vsite 
function update_block_ids(&$vsite) {
  $id_map = array(
    'vsite_widgets-2'				                  => 'shield_default',
    'vsite_widgets-0'				                  => 'site_logo',
    'vsite_widgets-1'				                  => 'site_info',
  	'views-scholar_announcements-block_1'  		=> 'scholar_announcements_latest',
  	'scholar_biocv-bio_blurb'									=> 'scholar_biocv_bio_blurb',
    'views-scholar_book-block_1'							=> 'scholar_book_recent_docs',
    'views-scholar_image_gallery-block_1'			=> 'image_gallery_recent_images',
  );
  $front_context = $vsite->controllers->context->get('openscholar-feature-front:reaction:block');
  $blocks = array();
  
  foreach ($front_context['blocks'] as $id => $a_block) {
    if (isset($id_map[$id])) { 
      $id = $id_map[$id];
      //      $box = boxes_load($id);
      //boxes_load uses space_get_space, which doesn't work here.  
      $box = $vsite->controllers->boxes->get($id);
      if (empty($box)) {
        module_load_include('inc', 'boxes', 'plugins/boxes_box');
        $box = boxes_box::load($id); 
      }
      drupal_alter('boxes_load', $box, $id);
    }
    
    //pass along the new block
    $new_front[$id] = os_boxes_get_vsite_widget($a_block, $a_block['weight'], $a_block['region']);
  }

  $front_context['blocks'] = $new_front;
  
  if ($vsite->controllers->context->set('openscholar-feature-front:reaction:block:'.$vsite->group->og_theme, $front_context)) {
    return $vsite->id;
  }
}
