<?php
/**
 * @file
 * os_links.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_links_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'links_links';
  $context->description = 'List of links or a page of type link';
  $context->tag = 'OS features';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'link' => 'link',
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
  t('List of links or a page of type link');
  t('OS features');
  $export['links'] = $context;

  return $export;
}
