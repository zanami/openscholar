<?php
/**
 * @file
 * os_booklets.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function os_booklets_user_default_permissions() {
  $permissions = array();

  // Exported permission: create booklet content.
  $permissions['create booklet content'] = array(
    'name' => 'create booklet content',
    'roles' => array(
      0 => 'administrator',
      1 => 'manager',
      2 => 'vsite admin',
      3 => 'vsite user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any booklet content.
  $permissions['delete any booklet content'] = array(
    'name' => 'delete any booklet content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own booklet content.
  $permissions['delete own booklet content'] = array(
    'name' => 'delete own booklet content',
    'roles' => array(
      0 => 'administrator',
      1 => 'manager',
      2 => 'vsite admin',
      3 => 'vsite user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any booklet content.
  $permissions['edit any booklet content'] = array(
    'name' => 'edit any booklet content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own booklet content.
  $permissions['edit own booklet content'] = array(
    'name' => 'edit own booklet content',
    'roles' => array(
      0 => 'administrator',
      1 => 'manager',
      2 => 'vsite admin',
      3 => 'vsite user',
    ),
    'module' => 'node',
  );

  return $permissions;
}
