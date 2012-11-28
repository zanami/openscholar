<?php
/**
 * @file
 * harvard_courses.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function harvard_courses_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'harvard_courses';
  $context->description = '';
  $context->tag = 'Harvard';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'courses' => 'courses',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-harvard_courses_filter' => array(
          'module' => 'boxes',
          'delta' => 'harvard_courses_filter',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Harvard');
  $export['harvard_courses'] = $context;

  return $export;
}
