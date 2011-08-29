<?php 

/**
 * @file batch functions for creating boxes from existing front pages
 */


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
  $_box_plugins = os_boxes_boxes_plugins() ;
  
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
      if (($box = fb_mk_front_box($vsite, $front)) != FALSE) {
        $context['results'][] = fb_add_box($vsite, $box);
        $context['message'] = 'Processing vsites ' . $offset . '-' . min($offset+$limit, $total_vsites) . ' of ' . $total_vsites;
        $context['sandbox']['progress']++;
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

// given db info on front page, return its settings.
function fb_get_front($settings, &$vsite) {
  $front = array();
  if (isset($settings['scholar_front_site_title'])) {
    $front['title'] = $settings['scholar_front_site_title'];
  }
  
  if (!count($settings)) {
  	return false; //no settings, no box
  	// or set them up with default bio box?  no, that should come in from presets
  }
  
  switch ($settings['site_frontpage']) {
    //features content
    case 'scholar_front/features':
      $front['type'] = 'features';
      $front['options']['feature_nodes'] =  $settings['scholar_front_feature_nodes'];
      //nid and path are useless
      break;
      
    //flagged content
    case 'scholar_front/flag':
      $front['type'] = 'flag';
      //no options
      break;
      
    //path
    case ''.$settings['scholar_front_frontpage_path']:
      $front['type'] = 'path';
      $front['options'] = array('path' => $settings['scholar_front_frontpage_path']);
      break;
      
    //create_bio -> bio node
  }
  
  if (isset($front['type'])) {
    return $front;
  }
  

  
  
  //no type yet.  frontpage is a node/nid.  maybe bio, maybe nid.
  $nid = (int) end(split('/', $settings['site_frontpage']));
  if (!$nid) {
  	//can't get a nid or nid is 0
  	  //node or path?
 	 $url = $vsite->get_absolute_url() . $settings['scholar_front_frontpage_path'];
  	$m = menu_get_item($url);
  	$p = drupal_lookup_path('source', $url);
  }
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
  $opts = array();
  global $_box_plugins;
  
  
  switch ($settings['type']) {
    case 'bio':
      $type = 'os_boxes_bio';
      $opts['nid'] = (int) end(explode('/', $settings['options']['node'])); //get nid from node/978897
      
      //or... check if bio has a blurb.  if it does, create a simple text box.  otherwise bio box
      break;
      
    case 'nid':
      $type = 'os_boxes_node';
      $opts['nid'] = (int) end(explode('/', $settings['options']['node'])); //get nid from node/978897
      break;
      
    case 'path':
      //path as a front page box is not supported.  
      return false;
      
    case 'flag':
      $type = 'os_boxes_featuredposts';
      break;
      
    case 'features':
      $type = 'os_boxes_features';
      if (isset($settings['options']['feature_nodes'])) {
        $opts['node_types'] = array_filter($settings['options']['feature_nodes']);
      } else {
      	//show all the features
      	vsite_include('vsiteapi');
		$node_types = vsite_content_types(array(2,1), $vsite);
        drupal_alter('vsite_content_types', $node_types);
        $node_types = array_combine( array_keys($node_types), array_keys($node_types) );
        asort($node_types);
        $opts['node_types'] = $node_types;
      }
      break;
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
  
  return mk_box($vsite, $type, $opts);
}

//creates a box object in the vsite given type and opts.  returns $box
function mk_box(&$vsite, $type, $opts) {
  //get tags.  boxes_factory omits them.
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

