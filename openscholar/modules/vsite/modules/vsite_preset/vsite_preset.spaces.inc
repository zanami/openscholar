<?php
/**
 * @file
 * vsite_preset.spaces.inc
 */

/**
 * Implements hook_spaces_presets().
 */
function vsite_preset_spaces_presets() {
  $export = array();

  $spaces_presets = new stdClass();
  $spaces_presets->disabled = FALSE; /* Edit this to true to make a default spaces_presets disabled initially */
  $spaces_presets->api_version = 3;
  $spaces_presets->name = 'os_department_administrative';
  $spaces_presets->title = 'Administrative Department';
  $spaces_presets->description = 'Standard information architecture for administrative sites.';
  $spaces_presets->space_type = 'og';
  $spaces_presets->value = '';

  // Translatables
  // Included for use with string extractors like potx.
  t('Administrative Department');
  t('Standard information architecture for administrative sites.');
  $export['os_department_administrative'] = $spaces_presets;

  return $export;
}
