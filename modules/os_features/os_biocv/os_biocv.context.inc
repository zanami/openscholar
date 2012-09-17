<?php
// $Id$


/**
 * @file
 * os_biocv.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_biocv_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  /* Edit this to true to make a default context disabled initially */
  $context->disabled = FALSE;
  $context->api_version = 3;
  $context->name = 'biocv_biocv';
  $context->description = '';
  $context->tag = 'OS features';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'bio' => 'bio',
        'cv' => 'cv',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
    'path' => array(
      'values' => array(
        'biocv' => 'biocv',
        'biocv/*' => 'biocv/*',
        'bio' => 'bio',
        'bio/*' => 'bio/*',
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
        'boxes-test_bio_box' => array(
          'module' => 'boxes',
          'delta' => 'test_bio_box',
          'region' => 'content_top',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('OS features');
  $export['biocv_biocv'] = $context;

  $context = new stdClass();
  /* Edit this to true to make a default context disabled initially */
  $context->disabled = FALSE;
  $context->api_version = 3;
  $context->name = 'biocv_front';
  $context->description = '';
  $context->tag = 'OS features';
  $context->conditions = array(
    'context' => array(
      'values' => array(
        '~os_sitewide_*' => '~os_sitewide_*',
      ),
    ),
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-test_bio_box' => array(
          'module' => 'boxes',
          'delta' => 'test_bio_box',
          'region' => 'content_top',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('OS features');
  $export['biocv_front'] = $context;

  return $export;
}

