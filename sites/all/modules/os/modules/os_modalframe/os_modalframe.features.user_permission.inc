<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function os_modalframe_user_default_permissions() {
  $permissions = array();

  // Exported permission: create free_image content
  $permissions['create free_image content'] = array(
    'name' => 'create free_image content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
      '2' => 'scholar admin',
      '3' => 'scholar user',
    ),
  );

  // Exported permission: delete own free_image content
  $permissions['delete own free_image content'] = array(
    'name' => 'delete own free_image content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
      '2' => 'scholar admin',
      '3' => 'scholar user',
    ),
  );

  // Exported permission: edit own free_image content
  $permissions['edit own free_image content'] = array(
    'name' => 'edit own free_image content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
      '2' => 'scholar admin',
      '3' => 'scholar user',
    ),
  );

  // Exported permission: override free_image authored by option
  $permissions['override free_image authored by option'] = array(
    'name' => 'override free_image authored by option',
    'roles' => array(),
  );

  // Exported permission: override free_image authored on option
  $permissions['override free_image authored on option'] = array(
    'name' => 'override free_image authored on option',
    'roles' => array(),
  );

  // Exported permission: override free_image promote to front page option
  $permissions['override free_image promote to front page option'] = array(
    'name' => 'override free_image promote to front page option',
    'roles' => array(),
  );

  // Exported permission: override free_image published option
  $permissions['override free_image published option'] = array(
    'name' => 'override free_image published option',
    'roles' => array(),
  );

  // Exported permission: override free_image revision option
  $permissions['override free_image revision option'] = array(
    'name' => 'override free_image revision option',
    'roles' => array(),
  );

  // Exported permission: override free_image sticky option
  $permissions['override free_image sticky option'] = array(
    'name' => 'override free_image sticky option',
    'roles' => array(),
  );

  return $permissions;
}
