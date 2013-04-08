<?php

include_once (drupal_get_path('module', 'vsite_register') . '/vsite_register.form.inc');

/**
 * Menu callback; provides vsite_register_form for authorized users.
 */
function os_pinserver_vsite_register_form_page() {
  global $user;

  // Handle users logged in already.
  if (!$user->uid){

	  // Sends user to pinserver if not logged-in locally or via the pinserver
	  if (!pinserver_check_status()) {
	    return pinserver_redirect();
	  }

	  // Redirects pinserver-logged-in user if they don't own a vsite.
	  if ($uid = os_pinserver_get_uid_from_huid()) {
	    // Logs the user in based on the HUID->UID association.
	    $user = user_load(array('uid' => $uid));
	    $user->login = time();
	    db_update('users')->fields(array('login' => $user->login))->condition('uid', $user->uid, '=')->execute();
	  }
  }

  //This will redirect the user to their site or return TRUE if they can create a site
  if(_os_pinserver_create_vsite_redirect()){
    drupal_get_form('vsite_register_form');
  }
}



/**
 * Check if user can create vsites or not, or requested a private file, and redirect appropriately.
 */
function _os_pinserver_create_vsite_redirect() {
  global $user;

  // Redirect user to requested private file (or other private destination).
  if (isset($_GET['redirect'])) {
    // Strip superfluous preceeding '/' before redirecting.
    return drupal_goto(preg_replace('|^/|', '', $_GET['redirect']));
  }

  $vsites = vsite_get_vsite_by_owner($user->uid);

  // Can user create vsites or not?
  if (vsite_vsite_exists_access($vsites)) {
    // Can create new vsites; provide form to create a new vsite.
    return TRUE;
  }
  else if (count($vsites)) {
    // Can't create new sites; redirect user to existing vsite(s).
    return _os_pinserver_existing_vsite_redirect($vsites);
  }

  return FALSE;  // This shouldn't happen.
}

/*
 * Redirect to the proper vsite.
*/
function _os_pinserver_existing_vsite_redirect($vsites) {
  // Redirects user to existing vsite(s).
  $count = count($vsites);

  drupal_get_messages();  // Clear message queue.
  drupal_set_message(t('You have already registered your maximum number of !count !websites.', array(
  '!count' => "<i>$count</i>",
  '!websites' => t('website' . ($count == 1 ? '' : 's')),
  )));

  if (count($vsites) == 1) {
    drupal_goto($vsites[0]->get_absolute_url());
  }
  else {
    drupal_goto('user');
  }
}