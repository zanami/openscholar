<?php

/**
 * @file
 * Page callback for the cp user admin functions.
 */
function cp_user_admin($callback_arg = '') {
  $op = isset($_POST['op']) ? $_POST['op'] : $callback_arg;
  
  $vsite = spaces_get_space();
  
  switch ($op) {
    case t('Add user'):
    case 'add':
      if($vsite){
        $build['user_add'] = drupal_get_form('cp_user_add_users','node', $vsite->id);
      }
      break;
    case t('Create new account'):
    case 'create':
      $build['user_register'] = drupal_get_form('user_register_form');
      break;
    case 'members':
    default:
      $build = cp_user_members();
  }
  return $build;
}

/**
 * Load the users control panel
 *
 * @todo Adapt view to work outside of a space as well
 */
function cp_user_members(){
  $vsite = spaces_get_space();
  
  $output = '';// "<p>".l('Add New Member','cp/users/add')."</p>";
  
  if($vsite){
    $view = views_get_view('os_site_members', 'default');
    $view->set_arguments(array('node', $vsite->id));
    $view->override_url = $_GET['q'];
    $output .= $view->preview();
  }
  
  return $output;
}

///**
// *
// */
//function cp_user_add_user(){
//  $vsite = spaces_get_space();
//
//  if($vsite){
//
//    return drupal_get_form('cp_user_add_users','node', $vsite->id);
//  }
//
//  return;
//
//}

/**
 * Add users to group form.
 */
function cp_user_add_users($form, &$form_state, $group_type, $gid) {
  
  $group = entity_load_single($group_type, $gid);
  $label = entity_label($group_type, $group);

  $form['group_type'] = array('#type' => 'value', '#value' => $group_type);
  $form['gid'] = array('#type' => 'value', '#value' => $gid);

  $form['og_user'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add an existing user to %group', array('%group' => $label)),
  );
  $form['og_user']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('User name'),
    '#autocomplete_path' => 'user/autocomplete',
    '#required' => TRUE,
  );
  $form['og_user']['state'] = array(
    '#type' => 'value',
    '#value' => OG_STATE_ACTIVE,
  );

  $field_names = og_get_group_audience_fields('user', 'user', FALSE);
  $field_name = !empty($form_state['values']['field_name']) ? $form_state['values']['field_name'] : key($field_names);

  if(count($field_names) == 1){
    $form['og_user']['field_name'] = array(
      '#type' => 'value',
      '#value' => $field_name,
    );
  }else{
  
    $form['og_user']['field_name'] = array(
      '#type' => 'select',
      '#title' => t('Field name'),
      '#options' => $field_names,
      '#default_value' => $field_name,
      '#description' => t('Select the field name, the group membership should be registered in.'),
    );
  }

  // Add group membership form.
  $values = array();

  // Add group membership form. We still don't have the user or state.
  $og_membership = og_membership_create($group_type, $gid, 'user', 0, $field_name, $values);
  $form_state['og_membership'] = $og_membership;

  $form['membership_fields'] = array(
    '#prefix' => '<div id="og-ui-field-name">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    '#parents' => array('membership_fields'),
  );
  field_attach_form('og_membership', $og_membership, $form['membership_fields'], $form_state);
  
  //Do not require a request message when adding users in a vsite (This would send a notification to group administrators)
  unset($form['membership_fields']['og_membership_request']);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Add user'));

  return $form;
}

/**
 * Validate handler; Add users to group.
 */
function cp_user_add_users_validate($form, &$form_state) {
  $account = user_load_by_name($form_state['values']['name']);
  $group_type = $form_state['values']['group_type'];
  $gid = $form_state['values']['gid'];
  $field_name = $form_state['values']['field_name'];
  $params = array('%name' => format_username($account));
  if (!$account) {
    form_set_error('name', t('You have entered an invalid user name.'));
    return;
  }
  elseif (og_get_membership($group_type, $gid, 'user', $account->uid)) {
    form_set_error('name', t('User %name is already subscribed to group.', $params));
  }
  // Check field cardinality.
  elseif (!og_check_field_cardinality('user', $account, $field_name)) {
    $instance = field_info_instance('user', $field_name, 'user');
    $params['%field'] = $instance['label'];
    form_set_error('field_name', t('%name has reached maximum values in field %field.', $params));
  }
  else {
    $og_membership = $form_state['og_membership'];
    field_attach_form_validate('og_membership', $og_membership, $form['membership_fields'], $form_state);
  }
}

/**
 * Submit handler; Add users to group.
 */
function cp_user_add_users_submit($form, &$form_state) {
  $group_type = $form_state['values']['group_type'];
  $gid = $form_state['values']['gid'];
  $state = $form_state['values']['state'];
  $og_membership = $form_state['og_membership'];
  // Set the user and state.
  field_attach_submit('og_membership', $og_membership, $form['membership_fields'], $form_state);
  $account = user_load_by_name($form_state['values']['name']);
  // Ungroup user, in case they were already registered.
  og_ungroup($group_type, $gid, 'user', $account);

  $og_membership->etid = $account->uid;
  $og_membership->state = $state;
  $og_membership->save();

  drupal_set_message(t('%user has been added to group.', array('%user' => format_username($account))));
}

/**
 * Add a new user to your site that does not already exist
 *
 * @todo Combine this with add user
 */
function cp_user_create_new_user(){
  
}