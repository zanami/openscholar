<?php

/**
 * Implementation of hook_default_box().
 */
function scholar_book_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'scholar_book_recent_docs';
  $box->plugin_key = 'os_boxes_simpleview_list';
  $box->title = 'Recent Documents';
  $box->description = 'Recent Documents';
  $box->options = array(
    'filter' => 'node:2',
    'style' => 'titles',
    'sort' => 'newest',
    'argument' => '',
    'number_of_items' => '6',
  );

  $export['scholar_book_recent_docs'] = $box;
  return $export;
}
