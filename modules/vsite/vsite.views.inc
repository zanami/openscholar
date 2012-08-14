<?php 

/**
 * Implements hook_views_handlers().
 */
function vsite_views_handlers() {
  return array(
      'info' => array(
          'path' => drupal_get_path('module', 'spaces') . '/includes',
      ),
      //Handler to link to a vsite using the computed "Homepage" path, stored on the vsite node
      'handlers' => array(
          'vsite_handler_field_node' => array('parent' => 'views_handler_field_node'),
      ),
  );
}

/**
 * Implements hook_views_data_alter().
 */
function vsite_views_data_alter(&$data) {
  
  // Iterate through Views data and replace instances of
  // views_handler_field_node with vsite node handler.
  // This allows easily linking to a vsite homepage via views
  foreach ($data as $table => $data_table) {
    foreach ($data_table as $field => $data_field) {
      if (isset($data_field['field']['handler']) && in_array($data_field['field']['handler'],array('views_handler_field_node','spaces_handler_field_node'))) {
        $data[$table][$field]['field']['handler'] = 'vsite_handler_field_node';
      }
    }
  }
}