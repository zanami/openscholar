<?php

/**
 * @file
 *
 * helper functions for creating new boxes and adding them to regions
 */

/**
 * @function mk_box
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
function mk_box(&$vsite, $type, $opts) {
  global $box_plugins;
  if (empty($box_plugins)) {
    $box_plugins  = os_boxes_boxes_plugins();
    $box_plugins += spaces_simpleviews_boxes_plugins();
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
 * @function vsite_put_box
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
function vsite_put_box(&$vsite, $box, $region) {

  //let spaces select the current setting or the default
  $front_context = $vsite->controllers->context->get('openscholar-feature-front:reaction:block');
  $front_context['blocks']['boxes-' . $box->delta] = array(
    'module' => 'boxes',
    'delta' => $box->delta,
    'weight' => 0,
    'region' => $region,
    'label' => $box->title,
    'block_config_path' => 'cp/osboxes/nojs/' . $box->delta . '/configure', //is this always correct?
    'tags' => $box->tags,
  );

  if ($vsite->controllers->context->set('openscholar-feature-front:reaction:block:'.$vsite->group->og_theme, $front_context)) {
    return $vsite->id;
  } else {
    return FALSE;
  }
}

/**
 * @function vsite_remove_box
 *    Wrapper for vsite_put_box instead removes display of the box
 */
function vsite_remove_box(&$vsite, $box) {
  vsite_put_box($vsite, $box, false);
}

/**
 * @function replace_boxes
 * 		Given a block id, replace it with the given block
 * @param $block_id - String id of the block to replace
 * @param $box - Object
 */
function replace_boxes($block_id, $box) {
  static $available_widgets = null;
  if (!isset($available_widgets)) {
    $available_widgets = module_invoke_all('vsite_widgets');
  }

  $q = db_query("SELECT * FROM {spaces_overrides} WHERE type = 'og' AND object_type = 'context' AND object_id LIKE '%%reaction:block%%'");
  while ($r = db_fetch_array($q)) {
    $value = unserialize($r['value']);
    $region = $value['blocks'][$block_id]['region'];
    unset($value['blocks'][$block_id]);
    $value['blocks']['boxes-'.$box->delta] = array('region' => $region) + $available_widgets['boxes-'.$box->delta];
    db_query("UPDATE {spaces_overrides} SET value = '%s' WHERE type = 'og' AND object_type = 'context' AND id = %d AND object_id = '%s'", serialize($value), $r['id'], $r['object_id']);
    $vsite = vsite_get_vsite($r['id']);
    $vsite->controllers->boxes->set($box->delta, $box);
  }
}

//add block function?  do we ever need that for migration or is that only for front page boxes?