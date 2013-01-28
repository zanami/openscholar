<?php
// $Id$

/**
 * @file
 * OpenScholar control panel looks here for feature settings form.
 */

/**
 * Provides settings form for this feature on the Control Panel.
 */
function os_files_private_settings() {
  $form = array();

  $form['os_files_private_link'] = array(
    '#type' => 'markup',
    '#prefix' => '<p>'
      . t('Private files are only accessible to your ' . l(t('site members'), '/cp/users')) . '.'
      . '</p>'
      . '<p>'
      . l(t('Add and manage private files'), OS_FILES_PRIVATE_PATH)
      . '</p>',
  );
  // Checks both that
  // - Pinserver module is enabled, and
  // - The current vsite has a value for the pinserver app name variable.
  $pinserver_app_name = variable_get('pinserver_app_name', '');
  if ($pinserver_app_name && module_exists('pinserver')) {
    $form['os_files_private_pinserver'] = array(
      '#type' => 'checkbox',
      '#title' => t('Extend access to Harvard Community'),
      '#default_value' => variable_get('os_files_private_pinserver', 0),
      '#description' => t('By default, private files are only accessible to your site members. By extending access to Harvard Community, anyone with a valid Harvard XID can log in using the Harvard Pinserver to access your private files.'),
      '#weight' => 1,
    );
  }

  // Allows administrators only to edit pinserver app name in spaces_overrides.
  // @todo deprecate per-site app names, replace with per-site config.
  global $user;
  if (user_access('administer pinserver')) {
    $form['pinserver_app_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Harvard PIN server application name'),
      '#required' => FALSE,
      '#default_value' => variable_get('pinserver_app_name'),
      '#description' => t('Enter the application name given to you by Harvard Directory Services.' ),
    );
  }

  return system_settings_form($form);
}