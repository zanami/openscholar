<?php
/**
 * @file
 * vsite.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function vsite_field_default_fields() {
  $fields = array();

  // Exported field: 'node-department-field_site_logo'.
  $fields['node-department-field_site_logo'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_site_logo',
      'foreign keys' => array(
        'fid' => array(
          'columns' => array(
            'fid' => 'fid',
          ),
          'table' => 'file_managed',
        ),
      ),
      'indexes' => array(
        'fid' => array(
          0 => 'fid',
        ),
      ),
      'locked' => '0',
      'module' => 'imagefield_crop',
      'settings' => array(
        'default_image' => 0,
        'uri_scheme' => 'public',
      ),
      'translatable' => '0',
      'type' => 'imagefield_crop',
    ),
    'field_instance' => array(
      'bundle' => 'department',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'image',
          'settings' => array(
            'image_link' => '',
            'image_style' => '',
          ),
          'type' => 'image',
          'weight' => 4,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_site_logo',
      'label' => 'Site Logo',
      'required' => FALSE,
      'settings' => array(
        'alt_field' => 0,
        'file_directory' => '',
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '',
        'max_resolution' => '',
        'min_resolution' => '',
        'title_field' => 0,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'module' => 'imagefield_crop',
        'settings' => array(
          'collapsible' => 2,
          'croparea' => '500x500',
          'custom_ratio' => '',
          'enforce_minimum' => TRUE,
          'enforce_ratio' => TRUE,
          'gif_processing' => 'convert',
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber',
          'resolution' => FALSE,
          'select_maximum_area' => TRUE,
          'validate_resolution' => 0,
        ),
        'type' => 'imagefield_crop_widget',
        'weight' => '33',
      ),
    ),
  );

  // Exported field: 'node-personal-field_site_logo'.
  $fields['node-personal-field_site_logo'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_site_logo',
      'foreign keys' => array(
        'fid' => array(
          'columns' => array(
            'fid' => 'fid',
          ),
          'table' => 'file_managed',
        ),
      ),
      'indexes' => array(
        'fid' => array(
          0 => 'fid',
        ),
      ),
      'locked' => '0',
      'module' => 'imagefield_crop',
      'settings' => array(
        'default_image' => 0,
        'uri_scheme' => 'public',
      ),
      'translatable' => '0',
      'type' => 'imagefield_crop',
    ),
    'field_instance' => array(
      'bundle' => 'personal',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'image',
          'settings' => array(
            'image_link' => '',
            'image_style' => '',
          ),
          'type' => 'image',
          'weight' => 4,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_site_logo',
      'label' => 'Site Logo',
      'required' => FALSE,
      'settings' => array(
        'alt_field' => 0,
        'file_directory' => '',
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '',
        'max_resolution' => '',
        'min_resolution' => '',
        'title_field' => 0,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'module' => 'imagefield_crop',
        'settings' => array(
          'collapsible' => 2,
          'croparea' => '500x500',
          'custom_ratio' => '',
          'enforce_minimum' => TRUE,
          'enforce_ratio' => TRUE,
          'gif_processing' => 'convert',
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber',
          'resolution' => '180x140',
          'select_maximum_area' => TRUE,
          'validate_resolution' => 0,
        ),
        'type' => 'imagefield_crop_widget',
        'weight' => '33',
      ),
    ),
  );

  // Exported field: 'node-project-field_site_logo'.
  $fields['node-project-field_site_logo'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_site_logo',
      'foreign keys' => array(
        'fid' => array(
          'columns' => array(
            'fid' => 'fid',
          ),
          'table' => 'file_managed',
        ),
      ),
      'indexes' => array(
        'fid' => array(
          0 => 'fid',
        ),
      ),
      'locked' => '0',
      'module' => 'imagefield_crop',
      'settings' => array(
        'default_image' => 0,
        'uri_scheme' => 'public',
      ),
      'translatable' => '0',
      'type' => 'imagefield_crop',
    ),
    'field_instance' => array(
      'bundle' => 'project',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'image',
          'settings' => array(
            'image_link' => '',
            'image_style' => '',
          ),
          'type' => 'image',
          'weight' => 6,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_site_logo',
      'label' => 'Site Logo',
      'required' => FALSE,
      'settings' => array(
        'alt_field' => 0,
        'file_directory' => '',
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '',
        'max_resolution' => '',
        'min_resolution' => '',
        'title_field' => 0,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'module' => 'imagefield_crop',
        'settings' => array(
          'collapsible' => 2,
          'croparea' => '500x500',
          'custom_ratio' => '',
          'enforce_minimum' => TRUE,
          'enforce_ratio' => TRUE,
          'gif_processing' => 'convert',
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber',
          'resolution' => FALSE,
          'select_maximum_area' => TRUE,
          'validate_resolution' => 0,
        ),
        'type' => 'imagefield_crop_widget',
        'weight' => '33',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Site Logo');

  return $fields;
}
