<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function scholar_reader_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'openscholar-feature-reader';
  $context->description = 'Feed Reader';
  $context->tag = 'Feature';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'feed' => 'feed',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'reader' => 'reader',
        'reader/*' => 'reader/*',
      ),
      'options' => array(),
    ),
    'views' => array(
      'values' => array(
        'scholar_reader:page_1' => 'scholar_reader:page_1',
      ),
      'options' => array(),
    ),
  );
  $context->reactions = array(
    'menu' => 'reader',
    'block' => array(
      'blocks' => array(
        'vsite_widgets-4' => array(
          'module' => 'vsite_widgets',
          'delta' => '2',
          'weight' => 20,
          'region' => 'right',
          'status' => '0',
          'label' => 'AddThis Button - Share posts using popular services',
          'type' => 'context_ui',
        ),
        'vsite_taxonomy-0' => array(
          'module' => 'vsite_taxonomy',
          'delta' => '0',
          'weight' => 20,
          'region' => 'right',
          'status' => '0',
          'label' => 'Categories',
          'type' => 'context_ui',
        ),
        'boxes-scholar_reader-latest_feed_items' => array(
          'module' => 'boxes',
          'delta' => 'scholar_reader-latest_feed_items',
          'weight' => 21,
          'region' => 'right',
          'status' => '0',
          'label' => 'Latest Feed Items',
        ),
        'boxes-scholar_reader-feeds_list' => array(
          'module' => 'boxes',
          'delta' => 'scholar_reader-feeds_list',
          'weight' => 22,
          'region' => 'right',
          'status' => '0',
          'label' => 'Feeds List',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Feature');
  t('Feed Reader');

  $export['openscholar-feature-reader'] = $context;
  return $export;
}
