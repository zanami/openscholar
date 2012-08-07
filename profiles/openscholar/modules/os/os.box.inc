<?php
/**
 * @file
 * os.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_addthis';
  $box->plugin_key = 'os_boxes_addthis';
  $box->title = '';
  $box->description = 'AddThis';
  $box->options = array(
    'visibility' => 'feature',
    'widget_type' => 'toolbox_small',
    'toolbox_services' => 'facebook,twitter,email,linkedin,google_plusone',
    'additional_classes' => '',
  );
  $export['os_addthis'] = $box;
}
