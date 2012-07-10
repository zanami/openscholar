<?php
/**
 * @file
 * os_image_gallery.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_image_gallery_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'gallery_gallery';
  $context->description = 'Image gallery or image page';
  $context->tag = 'OS features';
  $context->conditions = array(
    'menu' => array(
      'values' => array(
        'taxonomy/term/1' => 'taxonomy/term/1',
      ),
    ),
    'node' => array(
      'values' => array(
        'media_gallery' => 'media_gallery',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array();
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Image gallery or image page');
  t('OS features');
  $export['gallery_gallery'] = $context;

  return $export;
}
