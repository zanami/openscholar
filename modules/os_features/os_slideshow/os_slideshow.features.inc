<?php
/**
 * @file
 * os_slideshow.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function os_slideshow_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function os_slideshow_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function os_slideshow_node_info() {
  $items = array(
    'slideshow_slide' => array(
      'name' => t('Slideshow Image'),
      'base' => 'node_content',
      'description' => t('An image in a slideshow widget.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
