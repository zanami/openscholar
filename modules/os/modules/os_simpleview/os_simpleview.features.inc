<?php

/**
 * Implementation of hook_views_api().
 */
function os_simpleview_views_api() {
  return array(
    'api' => '2',
  );
}


/**
 * Implementation of hook_ctools_plugin_api().
 */
function os_simpleview_ctools_plugin_api($module, $api) {
  if ($module == 'os_simpleview' && $api == 'plugins') {
    return array('version' => 1);
  } elseif ($module == 'boxes') {
    return array('version' => 1);//, 'path' => drupal_get_path('module', 'os_simpleview') . '/plugins/boxes');
  }
}

/**
 * Implements hook_boxes_plugins().
 */
function os_simpleview_boxes_plugins() {
  $info = array();
  $info['os_simpleview_box'] = array(
      'title' => 'simpleview box',
      'handler' => array(
          'parent' => 'os_boxes_default',
          'class' => 'os_simpleview_box',
          'file' => 'os_simpleview.box.inc',
          'path' => drupal_get_path('module', 'os_simpleview') . '/plugins/boxes',
      ),
      'block_config_path' => 'os_simpleview/ajax',
      
  );
  return $info;
}
