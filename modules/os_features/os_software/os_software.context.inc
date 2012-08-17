<?php
/**
 * @file
 * os_software.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_software_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'software_software';
  $context->description = '';
  $context->tag = 'OS features';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'software_project' => 'software_project',
        'software_release' => 'software_release',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'software' => 'software',
        'software/*' => 'software/*',
      ),
    ),
    'views' => array(
      'values' => array(
        'os_software_projects:page' => 'os_software_projects:page',
        'os_software_releases:page' => 'os_software_releases:page',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-os_filter_by_category' => array(
          'module' => 'boxes',
          'delta' => 'os_filter_by_category',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
        'boxes-os_addthis' => array(
          'module' => 'boxes',
          'delta' => 'os_addthis',
          'region' => 'sidebar_second',
          'weight' => '-9',
        ),
        'views-e99bd9fc465ac189dce3b71ba9fe2c95' => array(
          'module' => 'views',
          'delta' => 'e99bd9fc465ac189dce3b71ba9fe2c95',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
    'menu' => 'software',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('OS features');
  $export['software_software'] = $context;

  return $export;
}
