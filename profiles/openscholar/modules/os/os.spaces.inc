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
      'spaces_features' => array(
        'os_front' => 1,
        'os_pages' => 1,
      ),
      'site_frontpage' => 'xxxx',
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
      'spaces_features' => array(
        'os_front' => 1,
        'os_pages' => 1,
      ),
      'site_frontpage' => 'home',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('OS Personal');
  $export['os_personal'] = $spaces_presets;

  return $export;
}
