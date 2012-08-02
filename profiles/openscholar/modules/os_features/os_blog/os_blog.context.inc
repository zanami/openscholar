<?php
/**
 * @file
 * os_blog.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_blog_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'blog_blog';
  $context->description = 'Archive of recent blog entries';
  $context->tag = 'OS features';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'blog' => 'blog',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'blog' => 'blog',
        'blog/*' => 'blog/*',
      ),
    ),
    'views' => array(
      'values' => array(
        'os_blog:page' => 'os_blog:page',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-os_blog_comments-block' => array(
          'module' => 'views',
          'delta' => 'os_blog_comments-block',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
        'views-os_blog-block' => array(
          'module' => 'views',
          'delta' => 'os_blog-block',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Archive of recent blog entries');
  t('OS features');
  $export['blog_blog'] = $context;

  return $export;
}
