<?php
// $Id$


/**
 * @file
 * os_publications.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function os_publications_user_default_permissions() {
  $permissions = array();

  // Exported permission: access biblio content.
  $permissions['access biblio content'] = array(
    'name' => 'access biblio content',
    'roles' => array(
      0 => 'administrator',
      1 => 'anonymous user',
      2 => 'authenticated user',
      3 => 'manager',
      4 => 'vsite admin',
      5 => 'vsite user',
      6 => 'content editor',
    ),
    'module' => 'biblio',
  );

  // Exported permission: administer biblio.
  $permissions['administer biblio'] = array(
    'name' => 'administer biblio',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'biblio',
  );

  // Exported permission: edit biblio authors.
  $permissions['edit biblio authors'] = array(
    'name' => 'edit biblio authors',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'biblio',
  );

  // Exported permission: import from file.
  $permissions['import from file'] = array(
    'name' => 'import from file',
    'roles' => array(
      0 => 'administrator',
      1 => 'manager',
      2 => 'vsite admin',
    ),
    'module' => 'biblio',
  );

  // Exported permission: show download links.
  $permissions['show download links'] = array(
    'name' => 'show download links',
    'roles' => array(
      0 => 'administrator',
      1 => 'anonymous user',
      2 => 'authenticated user',
      3 => 'manager',
      4 => 'vsite admin',
      5 => 'vsite user',
    ),
    'module' => 'biblio',
  );

  // Exported permission: show export links.
  $permissions['show export links'] = array(
    'name' => 'show export links',
    'roles' => array(
      0 => 'administrator',
      1 => 'anonymous user',
      2 => 'authenticated user',
      3 => 'manager',
      4 => 'vsite admin',
      5 => 'vsite user',
    ),
    'module' => 'biblio',
  );

  // Exported permission: show filter tab.
  $permissions['show filter tab'] = array(
    'name' => 'show filter tab',
    'roles' => array(
      0 => 'administrator',
      1 => 'anonymous user',
      2 => 'authenticated user',
      3 => 'manager',
      4 => 'vsite admin',
      5 => 'vsite user',
    ),
    'module' => 'biblio',
  );

  // Exported permission: show sort links.
  $permissions['show sort links'] = array(
    'name' => 'show sort links',
    'roles' => array(
      0 => 'administrator',
      1 => 'anonymous user',
      2 => 'authenticated user',
      3 => 'manager',
      4 => 'vsite admin',
      5 => 'vsite user',
    ),
    'module' => 'biblio',
  );

    // Exported permission: create biblio content.
  $permissions['create biblio content'] = array(
    'name' => 'create biblio content',
    'roles' => array(
      0 => 'administrator',
      1 => 'manager',
      2 => 'vsite admin',
      3 => 'vsite user',
      4 => 'content editor',
    ),
    'module' => 'node',
  );

  // Exported permission: delete biblios.
  $permissions['delete biblio entries'] = array(
    'name' => 'delete biblio entries',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own biblio content.
  $permissions['delete own biblio entries'] = array(
    'name' => 'delete own biblio entries',
    'roles' => array(
      0 => 'administrator',
      1 => 'manager',
      2 => 'vsite admin',
      3 => 'vsite user',
      4 => 'content editor',
    ),
    'module' => 'node',
  );

  // Exported permission: edit all biblio entries.
  $permissions['edit all biblio entries'] = array(
    'name' => 'edit all biblio entries',
    'roles' => array(
      0 => 'administrator',
      1 => 'content editor',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own biblio entries.
  $permissions['edit own biblio entries'] = array(
    'name' => 'edit own biblio entries',
    'roles' => array(
      0 => 'administrator',
      1 => 'manager',
      2 => 'vsite admin',
      3 => 'vsite user',
      4 => 'content editor',
    ),
    'module' => 'node',
  );

  // Exported permission: view full text
  $permissions['view full text'] = array(
    'name' => 'view full text',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'anonymous user',
      '2' => 'authenticated user',
      '3' => 'manager',
      '4' => 'vsite admin',
      '5' => 'vsite user',
      '6' => 'content editor',
    ),
    'module' => 'biblio',
  );

  return $permissions;
}

