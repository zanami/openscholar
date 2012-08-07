<?php
/**
 * @file
 * os.spaces.inc
 */

/**
 * Implements hook_spaces_presets().
 */
function os_spaces_presets() {
  $export = array();

  $spaces_presets = new stdClass();
  $spaces_presets->disabled = FALSE; /* Edit this to true to make a default spaces_presets disabled initially */
  $spaces_presets->api_version = 3;
  $spaces_presets->name = 'os_department';
  $spaces_presets->title = 'OS Department';
  $spaces_presets->description = '';
  $spaces_presets->space_type = 'og';
  $spaces_presets->value = array(
    'variable' => array(
      'pathauto_node_link_pattern' => '[vsite:site-purl]/links/[node:title]',
      'pathauto_node_announcement_pattern' => '[vsite:site-purl]/content/[node:title]',
      'pathauto_node_media_gallery_pattern' => '[vsite:site-purl]/content/[node:title]',
      'pathauto_node_biblio_pattern' => '[vsite:site-purl]/content/[node:title]',
      'pathauto_node_page_pattern' => '[vsite:site-purl]/content/[node:title]',
      'pathauto_node_event_pattern' => '[vsite:site-purl]/content/[node:title]',
      'pathauto_node_feed_pattern' => '[vsite:site-purl]/content/[node:title]',
      'pathauto_node_blog_pattern' => '[vsite:site-purl]/blog/[node:original:created:custom:Y]/[node:original:created:custom:m]/[node:title]',
      'pathauto_node_presentation_pattern' => '[vsite:site-purl]/presentations/[node:title]',
      'spaces_features' => array(
        'os' => '1',
        'os_pages' => '1',
        'os_announcements' => '1',
        'os_image_gallery' => '1',
        'os_links' => '1',
        'os_publications' => '1',
      ),
      'site_frontpage' => 'home',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('OS Department');
  $export['os_department'] = $spaces_presets;

  $spaces_presets = new stdClass();
  $spaces_presets->disabled = FALSE; /* Edit this to true to make a default spaces_presets disabled initially */
  $spaces_presets->api_version = 3;
  $spaces_presets->name = 'os_personal';
  $spaces_presets->title = 'OS Personal';
  $spaces_presets->description = '';
  $spaces_presets->space_type = 'og';
  $spaces_presets->value = array(
    'variable' => array(
      'pathauto_node_link_pattern' => '[vsite:site-purl]/links/[node:title]',
      'pathauto_node_announcement_pattern' => '[vsite:site-purl]/content/[node:title]',
      'pathauto_node_media_gallery_pattern' => '[vsite:site-purl]/content/[node:title]',
      'pathauto_node_biblio_pattern' => '[vsite:site-purl]/content/[node:title]',
      'pathauto_node_page_pattern' => '[vsite:site-purl]/content/[node:title]',
      'pathauto_node_event_pattern' => '[vsite:site-purl]/content/[node:title]',
      'pathauto_node_feed_pattern' => '[vsite:site-purl]/content/[node:title]',
      'pathauto_node_blog_pattern' => '[vsite:site-purl]/blog/[node:original:created:custom:Y]/[node:original:created:custom:m]/[node:title]',
      'pathauto_node_presentation_pattern' => '[vsite:site-purl]/presentations/[node:title]',
      'spaces_features' => array(
        'os' => '1',
        'os_pages' => '1',
        'os_announcements' => '1',
        'os_image_gallery' => '1',
        'os_links' => '1',
        'os_publications' => '1',
      ),
      'site_frontpage' => 'home',
      'theme_default' => 'cleanblue',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('OS Personal');
  $export['os_personal'] = $spaces_presets;

  return $export;
}
