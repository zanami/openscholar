<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function scholar_book_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'openscholar-feature-book';
  $context->description = 'Books';
  $context->tag = 'Feature';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'book' => 'book',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'documents' => 'documents',
        'documents/*' => 'documents/*',
      ),
      'options' => array(),
    ),
    'views' => array(
      'values' => array(
        'scholar_books:page_1' => 'scholar_books:page_1',
      ),
      'options' => array(),
    ),
  );
  $context->reactions = array(
    'menu' => 'documents',
    'block' => array(
      'blocks' => array(
        'scholar_book-booktoc' => array(
          'module' => 'scholar_book',
          'delta' => 'booktoc',
          'weight' => 20,
          'region' => 'right',
          'status' => '0',
          'label' => 'Scholar: Book TOC',
          'type' => 'context_ui',
        ),
        'boxes-os_addthis' => array(
          'module' => 'boxes',
          'delta' => 'os_addthis',
          'label' => 'AddThis Button',
          'weight' => 21,
          'region' => 'right',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Books');
  t('Feature');

  $export['openscholar-feature-book'] = $context;
  return $export;
}
