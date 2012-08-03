<?php
/**
 * @file
 * os_blog.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_blog_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'blog_addthis';
  $box->plugin_key = 'os_boxes_addthis';
  $box->title = '';
  $box->description = 'AddThis for blog';
  $box->options = array(
    'visibility' => 'feature',
    'widget_type' => 'toolbox_small',
    'toolbox_services' => 'facebook,twitter,email,linkedin,google_plusone',
    'additional_classes' => '',
  );
  $export['blog_addthis'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'blog_rss_feed';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'Blog RSS Feed';
  $box->options = array(
    'body' => array(
      'value' => '<p><a href="/blog/feed"><img src="/feed.png" alt="Subscribe" title="Subscribe" /> Subscribe</a></p>',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['blog_rss_feed'] = $box;

  return $export;
}
