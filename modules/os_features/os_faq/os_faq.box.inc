<?php

/**
 * @file
 * os_faq.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_faq_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_faq_recent_faqs';
  $box->plugin_key = 'os_sv_list_box';
  $box->title = 'Recent FAQs';
  $box->description = 'Recently posted frequently asked questions';
  $box->options = array(
    'content_type' => 'faq',
    'display' => 'display_title',
    'more' => 1,
    'more_text' => 'More',
    'more_link' => 'faq',
    'pager' => 0,
    'number_of_items' => '6',
    'sort_by' => 'sort_newest',
  );
  $export['os_faq_recent_faqs'] = $box;

  return $export;
}
