<?php 


/**
 * @file batch functions for creating boxes from existing front pages
 **/


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
  }
  
  //loop over all users
  $query = 'SELECT uid FROM {users} LIMIT %d OFFSET %d ';
  $result = db_query($query, (int)$limit, (int)$offset);
  while ($res = db_fetch_object($result)) {
    $vsites = vsite_get_vsite_by_owner($res->uid);
    //loop over this user's vsites
    foreach($vsites as $vsite) {
      $settings = fb_front_settings($vsite->id);
      $front = fb_get_front($settings, $vsite);
      
      if (($wgt = fb_mk_front_box($vsite, $front)) != FALSE) {
      	$add_func = 'fb_add_' . $wgt['wgt']; //box or block
        $context['results'][] = $add_func($vsite, $wgt['data']);

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
  } else {
    $front['type'] = 'nid';
    $front['options'] = array('node' => $settings['site_frontpage']);
  }  //case for isset $type and !isset $type ???
  
  // bio has site_frontpage of node/nid
  // unknown type does the same thing.
  
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
AND nid = %d
GROUP BY nid HAVING max(vid) 
EOD;
      $result = db_query($query, $opts['nid']);

      if ($r = db_fetch_object($result)) {	
      	//we have a blurb.  let's make a simple box for it. 
      	$type = 'os_boxes_simple';
      	$opts['body'] = $r->field_hook_text_value;
      	
      } else {
      	//nope, just a regular bio box
      	$type = 'os_boxes_bio';
      	$opts['nid'] = $nid;
      }
      
      break;
      
    //node box
    case 'nid':
      $type = 'os_boxes_node';
      $opts['nid'] = (int) end(explode('/', $settings['options']['node'])); //get nid from node/978897
      break;
      
      
    //no support for path boxes
    case 'path':
      //path as a front page box is not supported.  
      return false;
      
      
    //featuredposts block.  no new box supoport.  note flag to make this a block
    case 'flag':
      $type = 'os_boxes_featuredposts';
      break;
      
      
    //features uses simpleviews_boxes_ls
    case 'features':
      $type = 'spaces_simpleviews_boxes_ls';
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
  	$opts['title'] = $settings['type'] . ' box';
  	$opts['subject'] = '';
  }
  $opts['description'] = $_box_plugins[$type]['title'] . ' box';
  
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

//creates a box object in the vsite given type and opts.  returns $box
function mk_box(&$vsite, $type, $opts) {
  
  global $_box_plugins;
  $identifier = $vsite->type . '-' . $vsite->id;
  $hash = boxes_create_hash($identifier);
  $delta = $identifier . '-' . $hash;
  $opts['delta'] = $delta;
  
  if (($box = boxes_factory($type, $opts)) && isset($vsite->controllers->boxes)) { 
    $box->tags = $_box_plugins[$type]['tags'];
    $vsite->controllers->boxes->set($box->delta, $box);
    return $box;      
  } else {
    return false;
  }
}

// adds box to vsite.  returns id of vsite on successful creation
function fb_add_box(&$vsite, $box) {
  
  //let spaces select the current setting or the default
  $front_context = $vsite->controllers->context->get('openscholar-feature-front:reaction:block');
  $front_context['blocks']['boxes-' . $box->delta] = array(
    'module' => 'boxes',
    'delta' => $box->delta,
    'weight' => 0,
    'region' => 'content_top',   
    'label' => $box->title,
    'block_config_path' => 'cp/osboxes/nojs/' . $box->delta . '/configure',
    'tags' => $box->tags,
  );
  
  if ($vsite->controllers->context->set('openscholar-feature-front:reaction:block:'.$vsite->group->og_theme, $front_context)) {
    return $vsite->id;
  }
}

