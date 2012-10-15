<?php
/**
 * @file
 * harvard_courses_features.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function harvard_courses_features_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function harvard_courses_features_node_info() {
  $items = array(
    'harvard_api_importer' => array(
      'name' => t('Harvard API importer'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'harvard_course' => array(
      'name' => t('Harvard Course'),
      'base' => 'node_content',
      'description' => t('OpenScholar course import module.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
