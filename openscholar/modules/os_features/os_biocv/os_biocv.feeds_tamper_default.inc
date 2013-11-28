<?php
/**
 * @file
 * os_biocv.feeds_tamper_default.inc
 */

/**
 * Implements hook_feeds_tamper_default().
 */
function os_biocv_feeds_tamper_default() {
  $export = array();

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_bio-body-urf8_convert';
  $feeds_tamper->importer = 'os_bio';
  $feeds_tamper->source = 'Body';
  $feeds_tamper->plugin_id = 'urf8_convert';
  $feeds_tamper->settings = array();
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_bio-body-urf8_convert'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_bio-title-urf8_convert';
  $feeds_tamper->importer = 'os_bio';
  $feeds_tamper->source = 'Title';
  $feeds_tamper->plugin_id = 'urf8_convert';
  $feeds_tamper->settings = array();
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_bio-title-urf8_convert'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_cv-body-urf8_convert';
  $feeds_tamper->importer = 'os_cv';
  $feeds_tamper->source = 'Body';
  $feeds_tamper->plugin_id = 'urf8_convert';
  $feeds_tamper->settings = array();
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_cv-body-urf8_convert'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_cv-title-urf8_convert';
  $feeds_tamper->importer = 'os_cv';
  $feeds_tamper->source = 'Title';
  $feeds_tamper->plugin_id = 'urf8_convert';
  $feeds_tamper->settings = array();
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_cv-title-urf8_convert'] = $feeds_tamper;

  return $export;
}
