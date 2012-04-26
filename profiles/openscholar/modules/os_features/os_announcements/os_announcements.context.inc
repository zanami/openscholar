<?php
/**
 * @file
 * os_announcements.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_announcements_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'announcements_announcements';
  $context->description = 'Context that is active in the announcements section of your site';
  $context->tag = 'OS features';
  $context->conditions = array(
    'menu' => array(
      'values' => array(
        'announcements' => 'announcements',
      ),
    ),
    'node' => array(
      'values' => array(
        'announcement' => 'announcement',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        '/announcements' => '/announcements',
      ),
    ),
    'views' => array(
      'values' => array(
        'announcements_listing:page' => 'announcements_listing:page',
      ),
    ),
  );
  $context->reactions = array();
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context that is active in the announcements section of your site');
  t('OS features');
  $export['announcements_announcements'] = $context;

  return $export;
}
