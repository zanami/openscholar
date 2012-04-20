<?php
/**
 * @file
 * os_front.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'os_front';
  $context->description = 'Front Page';
  $context->tag = 'Feature';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'node-recent' => array(
          'module' => 'node',
          'delta' => 'recent',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Front Page');
  $export['os_front'] = $context;

  return $export;
}
