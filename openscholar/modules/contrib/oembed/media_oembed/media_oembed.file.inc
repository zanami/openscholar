<?php

/**
 * Implements hook_file_formatter_info().
 */
function media_oembed_file_formatter_info() {
  $formatters['oembed'] = array(
    'label' => t('oEmbed'),
    'default settings' => array('width' => '560', 'height' => '340', 'wmode' => ''),
    'view callback' => 'media_oembed_file_formatter_view',
    'settings callback' => 'media_oembed_file_formatter_oembed_settings',
  );
  $formatters['oembed_thumbnail'] = array(
    'label' => t('oEmbed Thumbnail'),
    'default settings' => array('width' => '180', 'height' => ''),
    'view callback' => 'media_oembed_file_formatter_view',
    'settings callback' => 'media_oembed_file_formatter_oembed_thumbnail_settings',
  );
  return $formatters;
}

/**
 * Implements hook_file_mimetype_mapping_alter().
 */
function media_oembed_file_mimetype_mapping_alter(&$mapping) {
  $mapping['mimetypes'][] = 'video/oembed';
  $mapping['mimetypes'][] = 'image/oembed';
  $mapping['mimetypes'][] = 'text/oembed';
}

/**
 * Implements hook_file_operations().
 */
function media_oembed_file_operation_info() {
  $operations = array(
    'refresh' => array(
      'label' => t('Refresh from source'),
      'callback' => 'media_oembed_cache_clear',
    ),
  );
  return $operations;
}
