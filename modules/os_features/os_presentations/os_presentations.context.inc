<?php
// $Id$


/**
 * @file
 * os_presentations.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_presentations_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  /* Edit this to true to make a default context disabled initially */
  $context->disabled = FALSE;
  $context->api_version = 3;
  $context->name = 'presentations_presentations';
  $context->description = 'Adds blocks to OS Presentation pages';
  $context->tag = 'OS features';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'presentation' => 'presentation',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'presentations' => 'presentations',
        'presentations/*' => 'presentations/*',
      ),
    ),
    'views' => array(
      'values' => array(
        'os_presentations' => 'os_presentations',
        'os_presentations:page' => 'os_presentations:page',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-os_taxonomy_fltr_cat' => array(
          'module' => 'boxes',
          'delta' => 'os_taxonomy_fltr_cat',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
        'boxes-os_addthis' => array(
          'module' => 'boxes',
          'delta' => 'os_addthis',
          'region' => 'sidebar_second',
          'weight' => '-9',
        ),
        'views-os_presentations-block' => array(
          'module' => 'views',
          'delta' => 'os_presentations-block',
          'region' => 'sidebar_second',
          'weight' => '-8',
        ),
      ),
    ),
    'menu' => 'presentations',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Adds blocks to OS Presentation pages');
  t('OS features');
  $export['presentations_presentations'] = $context;

  return $export;
}

