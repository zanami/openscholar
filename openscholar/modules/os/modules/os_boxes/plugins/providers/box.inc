<?php

$plugin = array(
  'title' => 'Box',
  'capture subpatterns' => TRUE,
  'scheme' => url('', array('absolute' => TRUE)) . 'box/*',
  'callback' => 'os_boxes_box_provider',
  'provider' => TRUE,
  'consumer' => TRUE
);


/**
 * Default provider for boxes
 */
function os_boxes_box_provider($plugin, $url, $matches, $parameters) {

  $result = FALSE;
  $delta = $matches[1];
  $defaults = array('view_mode' => 'full', 'langcode' => NULL);
  $parameters = array_merge($defaults, $parameters);
  $box = boxes_box_load($delta);
  if (!$box) {
    $s = db_select('spaces_overrides', 'so')
      ->fields('so', array('value'))
      ->condition('object_type', 'boxes')
      ->condition('object_id', $delta)
      ->execute()
      ->fetchField();
    $box = unserialize($s);
    $box = boxes_box::factory($box->plugin_key, $box);
  }
  if ($box) {
    module_load_include('module', 'block', 'block');
    $block = new stdClass();
    $block->bid = "boxes-{$box->delta}";
    $block->delta = $box->delta;
    $block->info = $box->description;
    $block->cache = DRUPAL_CACHE_CUSTOM;
    $block->module = 'boxes';

    $result = array(
      'type' => 'rich',
      'html' => $box->render(),
      'title' => $box->title,
      'widget_type' => $box->plugin_key
    );
    $result = _oembedprovider_result($result['type'], $result);
  }

  return $result;
}