<?php
/**
 * Functions and APIs for logging in a user via pinserver
 */

function _os_pinserver_auth_login_menu() {
  $items = array();

  $items['user/pin'] = array(
    'title' => 'Log in via PIN',
    'page callback' => 'os_pinserver_auth_pin_user_login',
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
function os_pinserver_auth_pin_user_login() {
 if ($errors = pinserver_get_errors()) {
    foreach ($errors as $e) {
      watchdog(WATCHDOG_ERROR, t('Pinserver authentication error: @error', array('@error' => $e)));
      error_log($e);
    }
  }
  if ($error = pinserver_check_system_settings()) {
    watchdog(WATCHDOG_ERROR, t('Pinserver connection error: @error', array('@error' => $error)));
    error_log($error);
    drupal_set_message('Unable to connect to pinserver. Contact your site administrator.', 'error');
    drupal_goto('user');
  }
  elseif (!pinserver_authenticate_user()) {
    drupal_set_message('sending to pin');
    $url = 'user/pin';
    $options = array(
      'absolute' => TRUE,
    );
    if (isset($_GET['destination'])) {
      $options['query']['destination'] = $_GET['destination'];
      unset($_GET['destination']);
    }
    $url = url($url, $options);
    drupal_set_message($url);
    pinserver_redirect(array('redirect' => $url));
  }
  elseif ($uid = os_pinserver_auth_get_uid_from_huid(pinserver_get_user_huid())) {
    drupal_set_message('existing pin user found');
    $GLOBALS['user'] = user_load($uid);
    user_login_finalize();
    drupal_goto('user');
  }
  else {
    drupal_set_message('new pin account needed');
    // Users needs to create a full account to continue further
    if (isset($_GET['destination'])) {
      $args = array('destination' => $_GET['destination']);
      unset($_GET['destination']);
    }
    else {
      $args = array('destination' => 'user');
    }
    drupal_goto('user/pin/create', array('query' => $args));
  }
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

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Create Account'),
    )
  );

  $form['#submit'] = array('os_pinserver_auth_user_create_form_submit');

  return $form;
}

function os_pinserver_auth_user_create_form_submit($form, &$form_state) {
  module_load_include('inc', 'os', 'includes/user');
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
 $tabs = &$data['tabs'][0]['output'];
    foreach ($tabs as $k => $t) {
      if ($t['#link']['path'] == 'user/pin') {
        $tabs[$k]['#link']['localized_options']['query'] = drupal_get_destination();
      }
    }
  }
}