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
  $spaces_presets->name = 'hwp_administrative';
  $spaces_presets->title = 'HWP Administrative';
  $spaces_presets->description = 'The Harvard Web Publishing standard Adminstrative Department site.';
  $spaces_presets->space_type = 'og';
  $spaces_presets->value = array(
    'bundles' => array('department'),
    'og_tasks' => array(
      'vsite_preset_section_about',
      'vsite_preset_section_news_&_events',
      'vsite_preset_section_resources',
      'vsite_preset_section_activities',
      'vsite_preset_section_services',
      'vsite_preset_section_home',
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
  t('HWP Administrative');
  t('The Harvard Web Publishing standard Administrative site.');
  $export['hwp_administrative'] = $spaces_presets;

  $spaces_presets = new stdClass();
  $spaces_presets->disabled = FALSE; /* Edit this to true to make a default spaces_presets disabled initially */
  $spaces_presets->api_version = 3;
  $spaces_presets->name = 'hwp_project';
  $spaces_presets->title = 'HWP Project';
  $spaces_presets->description = 'The Harvard Web Publishing standard Project site.';
  $spaces_presets->space_type = 'og';
  $spaces_presets->value = array(
    'bundles' => array('project'),
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
      'theme_default' => 'hwpi_modern',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('HWP Project');
  t('The Harvard Web Publishing standard Project site.');
  $export['hwp_project'] = $spaces_presets;

  $spaces_presets = new stdClass();
  $spaces_presets->disabled = FALSE; /* Edit this to true to make a default spaces_presets disabled initially */
  $spaces_presets->api_version = 3;
  $spaces_presets->name = 'hwp_lab_research_group';
  $spaces_presets->title = 'HWP Lab/Research Group';
  $spaces_presets->description = 'The Harvard Web Publishing standard Lab and Research Group site.';
  $spaces_presets->space_type = 'og';
  $spaces_presets->value = array(
    'bundles' => array('project'),
    'og_tasks' => array(
      'vsite_preset_section_research',
      'vsite_preset_section_resources',
      'vsite_preset_section_news_&_events',
      'vsite_preset_section_about',
      'vsite_preset_section_home',
    ),
    'variable' => array(
      'spaces_features' => array(
        'os' => '1',
        'os_pages' => '1',
        'os_news' => '1',
        'os_events' => '1',
        'os_profiles' => '1',
        'os_publications' => '1',
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
      'theme_default' => 'hwpi_modern',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('HWP Lab/Research Group');
  t('The Harvard Web Publishing standard Lab and Research Group site.');
  $export['hwp_lab_research_group'] = $spaces_presets;

  return $export;
}
