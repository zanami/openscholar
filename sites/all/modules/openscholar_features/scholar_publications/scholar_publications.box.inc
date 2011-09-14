<?php

/**
 * Implementation of hook_default_box().
 */
function scholar_publications_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'scholar_publications_recent';
  $box->plugin_key = 'spaces_simpleviews_post_list';
  $box->title = 'Recent Publications';
  $box->description = 'Recent Publications';
  $box->options = array(
    'filter' => 'node:16',
    'style' => 'titles',
    'sort' => 'newest',
    'argument' => '',
    'number_of_items' => '6',
  );

  $export['scholar_publications_recent'] = $box;
  return $export;
}
