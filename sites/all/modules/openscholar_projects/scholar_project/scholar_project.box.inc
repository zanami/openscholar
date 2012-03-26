<?php

/**
 * Implementation of hook_default_box().
 */
function scholar_project_default_box() {
  $export = array();

  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'projects_get_started';
  $box->plugin_key = 'os_boxes_simple';
  $box->title = '';
  $box->description = 'Front Page Text';
  $box->options = array(
    'body' => '&nbsp;',
    'format' => 0,
  );
  $export['projects_get_started'] = $box;

  return $export;
}
