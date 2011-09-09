<?php

/**
 * Implementation of hook_default_box().
 */
function os_boxes_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'shield_default';
  $box->plugin_key = 'os_boxes_shields';
  $box->title = '';
  $box->description = 'Shield';
  $box->options = array(
    'visibility' => 'feature',
    'shield' => 'sites/localhost.scholar/files/vsite_shields/harvard_shield-white.png',
  );

  $export['shield_default'] = $box;
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'site_logo';
  $box->plugin_key = 'os_boxes_modal_sitelogo';
  $box->title = '';
  $box->description = 'Site Logo';
  $box->options = array(
    'visibility' => 'feature',
  );

  $export['site_logo'] = $box;
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'site_info';
  $box->plugin_key = 'os_boxes_modal_siteinfo';
  $box->title = '';
  $box->description = 'Site Info';
  $box->options = array(
    'visibility' => 'feature',
  );

  $export['site_info'] = $box;
  return $export;
}
