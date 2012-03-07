<?php
/**
 * @file
 * os_front.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_front_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'openscholar-feature-front';
  $context->description = 'Front Page';
  $context->tag = 'Feature';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
      ),
    ),
  );
  $context->reactions = array();
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Feature');
  t('Front Page');
  $export['openscholar-feature-front'] = $context;

  return $export;
}
