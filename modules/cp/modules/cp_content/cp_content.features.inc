<?php
/**
 * @file
 * cp_content.features.inc
 */

/**
 * Implements hook_views_api().
 */
function cp_content_views_api() {
  return array(
    'version' => '3.0',
    'path' => drupal_get_path('module', 'cp_content') . '/views',
  );
}
