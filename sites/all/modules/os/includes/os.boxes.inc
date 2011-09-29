<?php

/**
 * @file
 *
 * helper functions for creating new boxes and adding them to regions
 */

/**
 * @function os_boxes_mk_box
 *   Creates a box object and adds it to a vsite
 *
 * @param &$vsite
 *   vsite to place the new box in
 *
 * @param $type
 *   Box plugin to use
 *
 * @param $opts
 *   Options to pass to new box
 *
 * @return
 *   Retruns box object or false if box couldn't be created
 *
 */
function os_boxes_mk_box(&$vsite, $type, $opts) {
  require_once drupal_get_path('module', 'boxes') . '/boxes.module';
  global $box_plugins;
  if (empty($box_plugins)) {
    $box_plugins  = os_boxes_get_boxes_plugins();
  }

  $identifier = $vsite->type . '-' . $vsite->id;
  $hash = boxes_create_hash($identifier);
  $delta = $identifier . '-' . $hash;
  $opts['delta'] = $delta;

  if (($box = boxes_factory($type, $opts)) && isset($vsite->controllers->boxes)) {
    $box->tags = $box_plugins[$type]['tags'];
    $vsite->controllers->boxes->set($box->delta, $box);
    return $box;
  } else {
    return false;
  }
}


/**
 * @function os_boxes_put_box
 *   Puts a box object in the specified region of a vsite
 *
 * @param &$vsite
 *   vsite to show box.  box must already belong to vsite.  this function affects the box's region within the vsite
 *
 * @param $box
 *   box object to put in a region
 *
 * @param $region
 *   section of the page to move the box to
 *
 */
// adds box to vsite.  returns id of vsite on successful creation
function os_boxes_put_box($context, &$vsite, $box, $region) {

  //let spaces select the current setting or the default
  $context_block_reaction = $vsite->controllers->context->get($context.':reaction:block');
  $context_block_reaction['blocks']['boxes-' . $box->delta] = array(
    'module' => 'boxes',
    'delta' => $box->delta,
    'weight' => 0,
    'region' => $region,
    'label' => $box->title,
    'block_config_path' => 'cp/osboxes/nojs/' . $box->delta . '/configure', //is this always correct?
    'tags' => $box->tags,
  );
  
  $og_theme = db_result(db_query('SELECT og_theme FROM {og} WHERE nid = %d', $vsite->group->nid));
  if($og_theme){
    $og_theme = ":{$og_theme}";
  }else{
    $og_theme = "";
  }

  if ($vsite->controllers->context->set('openscholar-feature-front:reaction:block'.$og_theme, $context_block_reaction)) {
    return $vsite->id;
  } else {
    return FALSE;
  }
}

/**
 * @function os_boxes_remove_box
 *    Wrapper for os_boxes_put_box instead removes display of the box
 */
function os_boxes_remove_box(&$vsite, $box) {
  os_boxes_put_box($vsite, $box, false);
}

/**
 * Remove a instance of a box that has been overridden or exists only in a vsite
 * @param vsite $vsite
 * @param box $box
 */
function os_boxes_remove_spaces_box_instance($vsite,$box){
	
	if(!isset($vsite->controllers->boxes) || !is_object($box)){
		return false;
	}
	
	return $vsite->controllers->boxes->del($box->delta);
}
/**
 * @function os_boxes_replace_boxes
 *    Given a block id, replace it with the given block
 * @param $block_id - String id of the block to replace
 * @param $box - Object (instance)
 * @param $default_region
 * @param $sid (the site to replace or all sites by default)
 */
function os_boxes_replace_boxes($block_id, $box, $default_region = false, $sid = false, $limit = false) {

  if (!$box->delta) {
    $box->delta = os_boxes_create_delta();
  }
  $widget_def = os_boxes_get_vsite_widget($box);

  $params = array($block_id);
  if($sid) $params[] = $sid;
  
  $sql_limit = ($limit)?"LIMIT {$limit} ":"";

  $q = db_query("SELECT * FROM {spaces_overrides} WHERE type = 'og' AND object_type = 'context' AND object_id LIKE '%%reaction:block%%' AND value LIKE '%%%s%%' ".($sid?" AND id = %d ":"").$sql_limit, $params);
  while ($r = db_fetch_array($q)) {
    $value = unserialize($r['value']);
    $region = $value['blocks'][$block_id]['region']?$value['blocks'][$block_id]['region']:$default_region;
    $weight = $value['blocks'][$block_id]['weight']?$value['blocks'][$block_id]['weight']:0;
    unset($value['blocks'][$block_id]);
    $value['blocks']['boxes-'.$box->delta] = array('region' => $region, 'weight' => $weight) + $widget_def;
    db_query("UPDATE {spaces_overrides} SET value = '%s' WHERE type = 'og' AND object_type = 'context' AND id = %d AND object_id = '%s'", serialize($value), $r['id'], $r['object_id']);
  }
}