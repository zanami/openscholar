<?php
/**
 * @file
 * vsite.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function vsite_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_site_info|node|personal|default';
  $field_group->group_name = 'group_site_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'personal';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Site Info',
    'weight' => '36',
    'children' => array(
      0 => 'field_site_address',
      1 => 'field_site_description',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Site Info',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_site_info|node|personal|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_site_info|node|personal|form';
  $field_group->group_name = 'group_site_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'personal';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Site Info',
    'weight' => '36',
    'children' => array(
      0 => 'field_site_address',
      1 => 'field_site_description',
      2 => 'title',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'show_label' => 1,
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_site_info|node|personal|form'] = $field_group;

  return $export;
}
