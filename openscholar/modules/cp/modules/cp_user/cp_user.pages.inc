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

  $name = _cp_user_get_full_name($user);

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
  else {
    $human_name = TRUE;
    $role = _cp_user_get_role($uid, $vsite->group->nid, $human_name);
    $role_edit = l('Edit', "cp/users/edit_membership/{$uid}");
    $status_edit = l('Remove', "cp/users/delete_membership/{$uid}");
  }

  $columns = array(
    'name' => $name,
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
  $header = array('Name', 'Role', 'Status');
  $rows = array();
  foreach ($members as $row => $columns) {
    $rows[] = array(
      $columns['name'],
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
  if (!isset($user_load['build_info']['args'][0]->uid)) {
    return;
  }
  $user = $user_load['build_info']['args'][0];

  // Prepares replacement value text for the modal form.
  $include_username = FALSE;
  $name = _cp_user_get_full_name($user, $include_username);
  $site = $space->group->title;

  // Form sets.
  $form['gid'] = array('#type' => 'value', '#value' => $space->group->nid);
  $form['bundle'] = array('#type' => 'value', '#value' => $space->group->type);
  $form['uid'] = array('#type' => 'value', '#value' => $user->uid);
  $form['user_name'] = array('#type' => 'value', '#value' => $name);
  $form['site_name'] = array('#type' => 'value', '#value' => $site);
  $form['title'] = array(
    '#type' => 'markup',
    '#markup' => t('<h2>Editing member <em>@name</em></h2>', array('@name' => $name)),
    '#weight' => 0,
  );
  $form['edit_membership'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role:'),
    '#weight' => 10,
  );
  // Prepares the list mode to generate verbose role descriptions on options.
  $list_mode = 'options';
  $form['edit_membership']['edit_role'] = array(
    '#type' => 'radios',
    '#options' => _cp_user_list_roles($list_mode),
    '#default_value' => _cp_user_get_role($user->uid, $space->group->nid),

  );

  // Builds and returns a confirmation form.
  $question = t('Editing members for site <em>@site</em>', array('@name' => $name, '@site' => $site));
  $path = 'cp/users';
  $description = ' ';
  $yes = t('Save');
  $no = t('Cancel');
  return confirm_form($form, $question, $path, $description, $yes, $no);
}

/**
 * Submit callback; updates roles after user confirms action.
 */
function cp_user_edit_membership_form_submit($form, &$form_state) {
  // Accesses values from the form.
  $bundle = $form_state['values']['bundle'];
  $new_role = $form_state['values']['edit_role'];
  $gid = $form_state['values']['gid'];
  $uid = $form_state['values']['uid'];
  // Grants the new role and revokes other roles.
  _cp_user_set_role($uid, $gid, $new_role, $bundle);

  // Set the message and redirect back to the cp/users page
  $user = $form_state['values']['user_name'];
  $site = $form_state['values']['site_name'];
  drupal_set_message(t('User <em>@user\'s</em> role on site @site was updated successfully.', array('@user' => $user, '@site' => $site)));
  drupal_goto('cp/users');
}

/**
 * Returns a name like "First Last (username)", or else username if none found.
 *
 * @param object $user
 *   The fully loaded user object.
 * @param bool $include_username
 *   If FALSE, users with full names will return "$first $last".
 *   If TRUE, users with full names will return "$first $last ($username)".
 *
 * @return string $name
 *   The user's full name, or username if none found.
 */
function _cp_user_get_full_name($user, $include_username = TRUE) {
  // Tries to get the user's first and last name, otherwise uses username.
  // Gets the first name field's safe value.
  $items = field_get_items('user', $user, 'field_first_name');
  if (isset($items[0]['safe_value'])) {
    $first_name = $items[0]['safe_value'];
  }
  else {
    $first_name = '';
  }
  // Gets the last name field's safe value.
  $items = field_get_items('user', $user, 'field_last_name');
  if (isset($items[0]['safe_value'])) {
    $last_name = $items[0]['safe_value'];
  }
  else {
    $last_name = '';
  }
  // Uses either the full name, or the username if first and last not available.
  if ($first_name && $last_name) {
    if ($include_username) {
      $name = "$first_name $last_name ({$user->name})";
    }
    else {
      $name = "$first_name $last_name";
    }
  }
  else {
    $name = $user->name;
  }

  return $name;
}

/**
 * Returns a themed label and description of a role for the Manage Members page.
 */
function _cp_user_role_option($label, $description) {
  return "<strong>$label</strong><br/>$description";
}

/**
 * Returns the user's current role for this group.
 *
 * @param bool $human_name
 *   If TRUE, returns the human-readable name (i.e. "Administrator")
 *   If FALSE, returns the machine name (i.e. "vsite admin")
 */
function _cp_user_get_role($uid, $gid, $human_name = FALSE) {
  // Loads the user's roles with this group node.
  $user_roles = og_get_user_roles('node', $gid, $uid);
  // Handles vsite admins...
  if (in_array('vsite admin', $user_roles)) {
    $role = OS_USER_ROLE_ADMIN;
  }
  // Handles non-admins...
  else if (in_array('content editor', $user_roles)) {
    $role = OS_USER_ROLE_EDITOR;
  }
  else if (in_array('vsite user', $user_roles)) {
    $role = OS_USER_ROLE_BASIC;
  }
  else {
    $role = t('authenticated user');
  }

  if ($human_name) {
    $names = _cp_user_list_roles();
    $role = isset($names[$role]) ? $names[$role] : t('None');
  }

  return $role;
}

/**
 * Returns an associative array mapping role names to human readable names.
 *
 * @param bool $verbose
 *   If TRUE, formats a 2-line label with descriptive help text for each role.
 *
 * @return array $roles
 */
function _cp_user_list_roles($mode = 'names', $bundle = NULL) {
  $roles = array();

  $machine_names = array(
    OS_USER_ROLE_BASIC,
    OS_USER_ROLE_EDITOR,
    OS_USER_ROLE_ADMIN,
  );
  $human_names = array(
    t('Basic member'),
    t('Content editor'),
    t('Administrator'),
  );

  switch ($mode) {
    case 'names':
      $roles = array_combine($machine_names, $human_names);
      break;

    case 'options':
      $descriptions = array(
        t('<em>Basic members</em> may only add new posts and edit their own posts.'),
        t('<em>Content editors</em> may add and edit posts, and also edit others\' posts.'),
        t('<em>Administrators</em> may add, edit or delete any post, and change site settings.'),
      );
      for ($i = 0; $i < sizeof($machine_names); $i++) {
        $roles[$machine_names[$i]] = _cp_user_role_option($human_names[$i], $descriptions[$i]);
      }
      break;

    case 'rids':
      $og_roles = og_roles('node', $bundle);
      for ($i = 0; $i < sizeof($machine_names); $i++) {
        $roles[$machine_names[$i]] = array_search($machine_names[$i], $og_roles);
      }
      break;
  }

  return $roles;
}

/**
 * Grants the specified role for this user on this group, and revokes others.
 * @param int $uid
 * @param int $gid
 * @param string $role_name
 */
function _cp_user_set_role($uid, $gid, $role_name, $bundle = 'personal') {
  $roles = _cp_user_list_roles('rids', $bundle);
  foreach ($roles as $machine_name => $human_name) {
    $rid = $roles[$role_name];
    if ($role_name == $machine_name) {
      $user_roles = og_get_user_roles('node', $gid, $uid);
      if (empty($user_roles[$rid])) {
        og_role_grant('node', $gid, $uid, $rid);
      }
    }
    else {
      og_role_revoke('node', $gid, $uid, $rid);
    }
  }
}