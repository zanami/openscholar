<?php

/**
 * @file
 * Provides and CRUD functions needed for adding or removing users from a vsite
 */


/**
 * Add users to a OpenScholar vsite
 *
 * @param $account
 * The user ID or object
 *
 * @param $site_id
 * Site ID
 *
 * @return bool
 * Success
 */
function vsite_user_add($account, $site_id = FALSE) {
  module_load_include('inc', 'vsite', 'includes/vsite');

  $account = is_object($account) ? $account : user_load($account);
  $vsite = vsite_get_vsite($site_id);

  // Add the user to this site.
  return vsite_add_entity($account, 'user', $vsite);
}

/**
 * Remove a user from a OpenScholar vsite
 *
 * @param $account
 * The user ID or object
 * @param $site_id
 * Site ID
 *
 * @return bool
 * Success
 */
function vsite_user_remove($account, $site_id) {
  module_load_include('inc', 'vsite', 'includes/vsite');

  $account = is_object($account) ? $account : user_load($account);
  $vsite = vsite_get_vsite($site_id);

  // Remove the user from this site.
  return vsite_remove_entity($account, 'user', $vsite);
}

/**
 * Grant a role to a user within a vsite.
 *
 * @param $uid
 *   The user ID.
 * @param $rid
 *   The role ID.
 * @param $site_id
 *   The Group ID
 * @param $group_type
 *   The Group Type
 */
function vsite_role_grant($uid, $role_name, $site_id = FALSE, $group_type = 'node') {
  _vsite_role_grant($uid, $role_name, $site_id, $group_type, 'revoke');
}

/**
 * Revoke a role to a user within a vsite.
 *
 * @param $uid
 *   The user ID.
 * @param $rid
 *   The role ID.
 * @param $site_id
 *   The Group ID
 * @param $group_type
 *   The Group Type
 */
function vsite_role_revoke($uid, $role_name, $site_id = FALSE, $group_type = 'node') {
  _vsite_role_manage($uid, $role_name, $site_id, $group_type, 'revoke');
}

/**
 * Helper function to grant or revoke an OG role.
 *
 * @param $uid
 *   The user ID.
 * @param $rid
 *   The role ID.
 * @param $site_id
 *   The Group ID
 * @param $group_type
 *   The Group Type
 * @param $operation
 *   The operation to perform. Defaults to "grant".
 */
function _vsite_role_manage($uid, $role_name, $site_id = FALSE, $group_type = 'node', $operation = 'grant') {
  if (!$site_id && $vsite = vsite_get_vsite()) {
    $site_id = $vsite->id;
    $group_type = $vsite->group_type;
  }

  if (!$site_id) {
    return;
  }

  $wrapper = entity_metadata_wrapper($group_type, $site_id);
  $bundle = $wrapper->getBundle();
  //Add role via OG
  $roles = og_roles($group_type, $bundle, $site_id);

  $function = $operation == 'grant' ? 'og_role_grant' : 'og_role_revoke';

  foreach ($roles as $rid => $role) {
    if ($role == $role_name){
      $function($group_type, $site_id, $uid, $rid);
    }
  }
}