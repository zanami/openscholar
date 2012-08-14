<?php
/**
 * @file
 * os_software.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_software_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_filter_by_category';
  $box->plugin_key = 'os_taxonomy_fbt';
  $box->title = 'Filter By Category';
  $box->description = 'Filter By Category Box';
  $box->options = array(
    'visibility' => 'feature',
    'additional_classes' => '',
  );
  $export['os_filter_by_category'] = $box;

  return $export;
}
