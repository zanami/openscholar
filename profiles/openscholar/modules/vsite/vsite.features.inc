<?php
/**
 * @file
 * vsite.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function vsite_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function vsite_node_info() {
  $items = array(
    'vsite' => array(
      'name' => t('OS Site'),
      'base' => 'node_content',
      'description' => t('A OpenScholar site that will hold content and have members.'),
      'has_title' => '1',
      'title_label' => t('Site Name'),
      'help' => '',
    ),
  );
  return $items;
}
