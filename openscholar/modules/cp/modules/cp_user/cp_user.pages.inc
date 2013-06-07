<?php

/**
 * Returns an array of links for this user in the Manage Members table.
 *
 * @param int $uid
 *   The specified user object's User ID.
 * @param int $gid
 *   The specified vsite object's OG group Node ID.
 *
 * @return array $columns
 */
function cp_user_get_member_row($uid) {
  $columns = array();

  // Loads the user object to get first and last names if available.
  $user = user_load($uid);
  $status = t('Active');
  // Get the first name field's safe value.
  $items = field_get_items('user', $user, 'field_first_name');
  if (isset($items[0]['safe_value'])) {
    $first_name = $items[0]['safe_value'];
  }
  else {
    $first_name = '';
  }

  // Get the last name field's safe value.
  $items = field_get_items('user', $user, 'field_last_name');
  if (isset($items[0]['safe_value'])) {
    $last_name = $items[0]['safe_value'];
  }
  else {
    $last_name = '';
  }

  // Checks to see if the user is the admin of this group.
  $vsite = spaces_get_space();
  // Checks if the current user is this vsite's author (owner).
  $is_owner = (bool) ($vsite->group->uid == $uid);
  // If the user is the owner, return a special row with no edit links.
  if ($is_owner) {
    $role = t('Manager');
    $role_edit = '';
    $status_edit = '';
  }

  // Otherwise, checks the current user's OG roles.
  // The member's role could be one of: Vsite admin, Content Editor, or Member.
  // @fixme determine better name for non-admin, non-content-editor
  else {
    // Loads the user's roles with this group node.
    $user_roles = og_get_user_roles('node', $vsite->group->nid, $uid);
    // Handles vsite admins...
    if (in_array('vsite admin', $user_roles)) {
      $role = t('Administrator');
    }
    // Handles non-admins...
    else if (in_array('Content editor', $user_roles)) {
      $role = t('Content editor');
    }
    else {
      $role = t('Member');
    }
    $role_edit = l('Edit', "cp/users/edit_membership/{$uid}");
    $status_edit = l('Remove', "cp/users/delete_membership/{$uid}");
  }

  $columns = array(
    'username' => $user->name,
    'first_name' => $first_name,
    'last_name' => $last_name,
    'role' => $role . $role_edit,
    'status' => $status . $status_edit,
  );

  return $columns;
}

/**
 * Returns an array containing the given vsite's members names and roles.
 *
 * @param int $gid
 *   The specified vsite object's OG group Node ID.
 *
 * @return array $members
 *   The list of members in the specified vsite.
 */
function cp_user_get_members($gid) {
  $query = db_select("og_membership", "ogm");
  $query->condition("ogm.gid", $gid, "=");
  $query->condition("entity_type", "user", "=");
  $query->fields("ogm", array("entity_type", "etid"));
  $results = $query->execute()->fetchAll();

  $members = array();
  foreach ($results as $record => $values) {
    $uid = $values->etid;
    $members[] = cp_user_get_member_row($uid);
  }
  return $members;
}

/**
 * Page callback; returns content for the CP > People page (Manage Members).
 *
 * @param int $gid
 *   The specified vsite object's OG group Node ID.
 */
function cp_user_user_management_page($gid) {
  // Loads information for each member of the vsite.
  $members = cp_user_get_members($gid);

  // Prepares the member table.
  $header = array('Username', 'First Name', 'Last Name', 'Role', 'Status');
  $rows = array();
  foreach ($members as $row => $columns) {
    $rows[] = array(
      $columns['username'],
      $columns['first_name'],
      $columns['last_name'],
      $columns['role'],
      $columns['status'],
    );
  }
  $table = theme('table', array('header' => $header, 'rows' => $rows));

  // Prepares the add new user link.
  $add_new_link = l(t('+ Add a user to your site'), 'cp/users/add');

  // Prepares the page content to output.
  $output = '<div class="cp-manage-users-wrapper"><div class="cp-add-new-user"><ul><li>' . $add_new_link . '</li></ul></div><div class="cp-manager-user-content">';
  $output .= $table;
  $output .= '</div></div>';

  return $output;
}

/**
 * Form builder; provides confirmation for "Remove Member" action.
 */
function cp_user_remove_membership_confirm($form_state, $user_load) {
  $space = spaces_get_space();
  // Form sets.
  $form['gid'] = array('#type' => 'value', '#value' => $space->group->nid);
  $form['account'] = array('#type' => 'value', '#value' => $user_load['build_info']['args'][0]->uid);
  $form['username'] = array('#type' => 'value', '#value' => $user_load['build_info']['args'][0]->name);
  $form['site_name'] = array('#type' => 'value', '#value' => $space->group->title);
  // Return hook_confirm_form();
  return confirm_form($form,
    t('Are you sure you want to remove !name from the website: %title?', array('!name' => $user_load['build_info']['args'][0]->name, '%title' => $space->group->title)),
    "cp/users",
    ' ',
    t('Remove'),
    t('Cancel')
  );
}

/**
 * Submit callback; removes the specified member after user confirms action.
 */
function cp_user_remove_membership_confirm_submit($form, &$form_state) {
  // Ungroup the user from the node
  og_ungroup('node', $form_state['values']['gid'], 'user', $form_state['values']['account']);
  // Set the message and redirect back to the cp/users page
  drupal_set_message(t('!user has been removed from !website', array('!user' => $form_state['values']['username'], '!website' => $form_state['values']['site_name'])));
  drupal_goto('cp/users');
}

/**
 * Form builder; provides the options for the "Edit membership" form.
 */
function cp_user_edit_membership_form($form_state, $user_load) {
  $space = spaces_get_space();
  // Form sets.
  $form['gid'] = array('#type' => 'value', '#value' => $space->group->nid);
  $form['group_type'] = array('#type' => 'value', '#value' => $space->group->type);
  $form['account'] = array('#type' => 'value', '#value' => $user_load['build_info']['args'][0]->uid);
  $form['username'] = array('#type' => 'value', '#value' => $user_load['build_info']['args'][0]->name);
  $form['site_name'] = array('#type' => 'value', '#value' => $space->group->title);
  // Return hook_confirm_form();
  return confirm_form($form,
    t('Are you sure you want to make !name a group administrator for the website: %title?', array('!name' => $user_load['build_info']['args'][0]->name, '%title' => $space->group->title)),
    "cp/users",
    ' ',
    t('Confirm'),
    t('Cancel')
  );
}

/**
 * Submit callback; updates roles after user confirms action.
 */
function cp_user_edit_membership_confirm_submit($form, &$form_state) {
  // Get the vsite admin key.
  $og_roles = og_roles('node', $form_state['values']['group_type']);
  $key = array_search('vsite admin', $og_roles);
  $vsite_admin_role = $key;

  // Revoke the admin role
  og_role_revoke('node', $form_state['values']['gid'], $form_state['values']['account'], $vsite_admin_role);
  // Set the message and redirect back to the cp/users page
  drupal_set_message(t('!user has been removed as administrator for !website', array('!user' => $form_state['values']['username'], '!website' => $form_state['values']['site_name'])));
  drupal_goto('cp/users');
}