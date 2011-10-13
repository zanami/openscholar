<?php

/**
 * Implementation of hook_filefield_paths_item_default_items().
 */
function os_modalframe_filefield_paths_item_default_items() {
  $settings = array();

  // Exported free_image-field_free_image
  $settings['free_image-field_free_image'] = array(
    'type' => 'free_image',
    'field' => 'field_free_image',
    'filename' => array(
      'value' => '[filefield-onlyname-original].[filefield-extension-original]',
      'tolower' => 0,
      'pathauto' => 0,
      'transliterate' => 0,
    ),
    'filepath' => array(
      'value' => '[space-og-path-raw]/files',
      'tolower' => 0,
      'pathauto' => 0,
      'transliterate' => 0,
    ),
    'filealias' => array(
      'value' => '[space-og-path-raw]/files/[filefield-onlyname-original].[filefield-extension-original]',
      'tolower' => 0,
      'pathauto' => 0,
      'transliterate' => 0,
      'display' => 1,
    ),
  );

  return $settings;
}
