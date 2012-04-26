<?php
/**
 * @file
 * os.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function os_user_default_permissions() {
  $permissions = array();

  // Exported permission: administer nodes.
  $permissions['administer nodes'] = array(
    'name' => 'administer nodes',
    'roles' => array(
      0 => 'vsite admin',
    ),
    'module' => 'node',
  );

  return $permissions;
}
