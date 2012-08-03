<?php
/**
 * @file
 * os_blog.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function os_blog_user_default_permissions() {
  $permissions = array();

  // Exported permission: create blog content.
  $permissions['create blog content'] = array(
    'name' => 'create blog content',
    'roles' => array(
      0 => 'administrator',
      1 => 'manager',
      2 => 'vsite admin',
      3 => 'vsite user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own blog content.
  $permissions['delete own blog content'] = array(
    'name' => 'delete own blog content',
    'roles' => array(
      0 => 'administrator',
      1 => 'manager',
      2 => 'vsite admin',
    ),
    'module' => 'node',
  );

  return $permissions;
}
