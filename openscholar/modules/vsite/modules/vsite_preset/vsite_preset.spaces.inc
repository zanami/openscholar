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
  $spaces_presets->value = array(
    'bundles' => array('department'),
    'og_tasks' => array(
      'section_home',
      'section_about',
      'section_newsevents',
    ),
    'variable' => array(
      'spaces_features' => array(
        'os' => '1',
        'os_pages' => '1',
        'os_news' => '1',
        'os_events' => '1',
        'os_profiles' => '1',
      ),
      'spaces_features_available' => array(
        'os_blog',
        'os_booklets',
        'os_classes',
        'os_dataverse',
        'os_events',
        'os_faq',
        'os_files_private',
        'os_image_gallery',
        'os_links',
        'os_news',
        'os_pages',
        'os_presentations',
        'os_profiles',
        'os_publications',
        'os_reader',
        'os_software',
        'harvard_courses',
      ),
      'site_frontpage' => 'home',
      'theme_default' => 'hwpi_classic',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Administrative Department');
  t('Standard information architecture for administrative sites.');
  $export['os_department_administrative'] = $spaces_presets;

  return $export;
}
