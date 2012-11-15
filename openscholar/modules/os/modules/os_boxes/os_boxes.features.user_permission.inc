<?php
/**
 * @file
 * os_boxes.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function os_boxes_user_default_permissions() {
  $permissions = array();

  // Exported permission: administer boxes.
  $permissions['administer boxes'] = array(
    'name' => 'administer boxes',
    'roles' => array(
      0 => 'administrator',
      1 => 'manager',
    ),
    'module' => 'boxes',
  );

  return $permissions;
}
