<?php
/**
 * @file
 * vsite.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function vsite_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'os_sitewide_front';
  $context->description = 'Active on the frontpage in multi-tenant installs';
  $context->tag = 'OpenScholar';
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
        'views-os_websites-block' => array(
          'module' => 'views',
          'delta' => 'os_websites-block',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Active on the frontpage in multi-tenant installs');
  t('OpenScholar');
  $export['os_sitewide_front'] = $context;

  return $export;
}
