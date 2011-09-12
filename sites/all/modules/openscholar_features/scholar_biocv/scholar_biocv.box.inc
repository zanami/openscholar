<?php

/**
 * Implementation of hook_default_box().
 */
function scholar_biocv_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'scholar_biocv_bio_blurb';
  $box->plugin_key = 'os_boxes_bio';
  $box->title = 'Bio Blurb';
  $box->description = 'Bio Blurb';
  $box->options = array(
    'visibility' => 'feature',
    'nid' => 0,
    'teaser' => 'teaser',
  );

  $export['scholar_biocv_bio_blurb'] = $box;
  return $export;
}
