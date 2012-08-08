<?php
/**
 * @file
 * os_presentations.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_presentations_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_taxonomy_fltr_cat';
  $box->plugin_key = 'os_taxonomy_fbt';
  $box->title = '';
  $box->description = 'Filter by Category';
  $box->options = array(
    'visibility' => 'feature',
    'additional_classes' => '',
  );
  $export['os_taxonomy_fltr_cat'] = $box;

  return $export;
}
