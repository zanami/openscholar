<?php
/**
 * Functions and APIs for logging in a user via pinserver
 */

function _os_pinserver_auth_login_menu() {
  $items = array();

  $items['user/pin'] = array(
    'title' => 'Log in via PIN',
    'page callback' => 'os_pinserver_auth_user_login',
    'access callback' => 'os_pinserver_auth_user_login_access',
    'type' => MENU_LOCAL_TASK,
    'file' => 'includes/os_pinserver_auth.login.inc',
  );

  $items['user/pin/create'] = array(
    'title' => 'Create user from PIN',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('os_pinserver_auth_user_create_form'),
    'access callback' => 'os_pinserver_auth_user_login_access'
  );

  return $items;
}

/**
 * Redirect user to /user if they are already logged in
 * and they have already filled out the required new user form
 * @return bool
 */
function os_pinserver_auth_user_login_access() {
  if (user_is_logged_in()) {
    drupal_goto('user');
  }

  return true;
}

/**
 * Redirects to pinserver if they are not logged in
 * Prompts to
 */
function os_pinserver_auth_user_login() {
  pinserver_authenticate_user();
  if ($error = pinserver_check_system_settings()) {
    dpm($error);
    return '';
  }
  elseif ($errors = pinserver_get_errors()) {
    dpm($errors);
    return '';
  }
  elseif (!user_is_logged_in() && !pinserver_check_status()) {
    $url = 'user/pin';
    $url = url($url, array('absolute' => TRUE));
    pinserver_redirect(array('redirect' => $url));
  }

  $pin_user = pinserver_get_user();
  // user already exists. Log them in and go straight to their destination
  if ($uid = os_pinserver_auth_get_uid_from_huid($pin_user['huid'])) {
    $GLOBALS['user'] = user_load($uid);
    user_login_finalize();
    drupal_goto();
  }

  // Users needs to create a full account to continue further
  $args = drupal_get_destination();
  unset($_GET['destination']);
  drupal_goto('user/pin/create', array('query' => $args));
}

/**
 * Allows user to fill in the rest of their user account
 */
function os_pinserver_auth_user_create_form($form, &$form_state) {
  $pin_user = pinserver_get_user();

  $form['notice'] = array(
    '#markup' => t('This is your first time logging into OpenScholar through PIN.
      Please create an OpenScholar account before continuing. The next time you
      visit an OpenScholar site, you can log in with this or through PIN.')
  );

  module_load_include('form.inc', 'vsite_register');
  $form['user'] = _vsite_register_user_form();

  $form['#submit'] = array('os_pinserver_auth_user_create_form_submit');

  return $form;
}

function os_pinserver_auth_user_create_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $user_options = array(
    'name' => $values['name'],
    'pass' => $values['password'],
    'mail' => $values['mail'],
    'status' => 1,
    'field_first_name' => $values['first_name'],
    'field_last_name' => $values['last_name'],
  );
  $user = os_user_create($user_options);
  os_pinserver_auth_add_row($user->uid, pinserver_get_user_huid());

  $GLOBALS['user'] = $user;
  user_login_finalize();
}

function os_pinserver_auth_menu_local_tasks_alter(&$data, $router_item, $root_path) {
  if ($root_path == 'user') {
    $args = func_get_args();
    dpm($args);
  }
}