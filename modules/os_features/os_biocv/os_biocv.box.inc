<?php
// $Id$


/**
 * @file
 * os_biocv.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_biocv_default_box() {
  $export = array();

  $box = new stdClass();
  /* Edit this to true to make a default box disabled initially */
  $box->disabled    = FALSE;
  $box->api_version = 1;
  $box->delta       = 'test_bio_box';
  $box->plugin_key  = 'os_boxes_bio';
  $box->title       = '';
  $box->description = 'Bio Teaser';
  $box->options     = array(
    'additional_classes' => '',
  );
  $export['test_bio_box'] = $box;

  return $export;
}