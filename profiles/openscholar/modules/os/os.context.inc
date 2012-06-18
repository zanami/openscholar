<?php
/**
 * @file
 * os.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_context_default_contexts() {
  $export = array();
  
  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'os_cp';
  $context->description = 'Context that is active when in administrative areas of an Openscholar site, like the CP';
  $context->tag = 'OpenScholar';
  $context->conditions = array(
    'node' => array(
      'values' => array_keys(os_get_bundles(true)), //All OS Nodes
      'options' => array(
        'node_form' => '2',
      ),
    ),
    'path' => array(
      'values' => array(
        'cp' => 'cp',
        'cp/*' => 'cp/*',
      ),
    ),
  );
  $context->reactions = array();
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context that is active when in administrative areas of an Openscholar site, like the CP');
  t('OpenScholar');
  $export['os_cp'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'os_front';
  $context->description = 'Front Page';
  $context->tag = 'OS features';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
      ),
    ),
    'context' => array(
      'values' => array(
        '~os_sitewide_*' => '~os_sitewide_*',
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
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Front Page');
  t('OS features');
  $export['os_front'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'os_public';
  $context->description = 'Context that is active in public parts of an OpenScholar site';
  $context->tag = 'OpenScholar';
  $context->conditions = array(
    'context' => array(
      'values' => array(
        '~os_cp' => '~os_cp',
      ),
    ),
    'path' => array(
      'values' => array(
        '~cp/*' => '~cp/*',
        '~admin/*' => '~admin/*',
      ),
    ),
    'os_site' => array(
      'values' => array(
        '1' => '1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'os-secondary-menu' => array(
          'module' => 'os',
          'delta' => 'secondary-menu',
          'region' => 'sidebar_first',
          'weight' => '-10',
        ),
        'os-primary-menu' => array(
          'module' => 'os',
          'delta' => 'primary-menu',
          'region' => 'menu_bar',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context that is active in public parts of an OpenScholar site');
  t('OpenScholar');
  $export['os_public'] = $context;

  return $export;
}
