<?php

/**
 * Default front page, stub
 */
function os_frontpage(){

  return " ";
}

/**
 * 404 page
 */
function os_page_not_found_page() {
  $t = '<strong>' . t('This page can not be found.') . '</strong>';
  $t = t('The URL might be incorrect or have changed.  Please go back to the !link',
      array('!link' => l('homepage', '<front>')));
  return $t;
}

/**
 * User Page (replacement for core /user)
 *
 * This adds redirection if you click a stale login link
 */
/**
 * Access callback for path /user.
 *
 * Displays user profile if user is logged in, or login form for anonymous
 * users.
 */
function os_user_page() {
  global $user;
  module_load_include('pages.inc', 'user');

  if ($user->uid && isset($_GET['destination']) && $_GET['destination'] != 'user'){
    //Redirect to destination
    drupal_goto();
  }elseif ($user->uid) {
    menu_set_active_item('user/' . $user->uid);
    return menu_execute_active_handler(NULL, FALSE);
  }
  else {
    return drupal_get_form('user_login');
  }
}

/**
 * @function os_acquia_backups_page
 * 
 * Makes our backups available to certain users
 */
function os_acquia_backups_page($filename = NULL) {
  $_SERVER['AH_SITE_NAME'] = 'foo'; 
  $_SERVER['AH_SITE_ENVIRONMENT'] = 'bar';
  if (!isset($_SERVER['AH_SITE_NAME'], $_SERVER['AH_SITE_ENVIRONMENT'])) {
    return t('Not an acquia site');
  }
  
  $path = '/mnt/gfs/' . $_SERVER['AH_SITE_NAME'] . '.' . $_SERVER['AH_SITE_ENVIRONMENT'];
  $files = glob($path . '/*.sql.gz');
  
  $file = ($filename) ? "${path}/${filename}" : FALSE;
  if ($file && in_array($file, $files)) {
    header('Content-Description: File Transfer');
    header('Content-Type: application/octet-stream');
    header('Content-Disposition: attachment; filename='.basename($file));
    header('Content-Transfer-Encoding: binary');
    header('Expires: 0');
    header('Cache-Control: must-revalidate');
    header('Pragma: public');
    header('Content-Length: ' . filesize($file));
    ob_clean();
    flush();
    readfile($file);
    exit;
  } else {
    drupal_set_message(t('Could not find file: @filename', array('@filename' => $filename)), 'error');
    //$list_items = array_map(create_function('$a', '$info = pathinfo($a); return l($info["basename"], "acquia_backups/".$info["basename"]);'), $files);
    $list_items = array_map('_os_acquia_backups_page_theme_file', $files);
    return theme('item_list', array('items' => $list_items, 'type' => 'ul'));
  }
}

/**
 * @function _os_acquia_backups_page_theme_file
 *
 * Returns formatted version of filename
 */
function _os_acquia_backups_page_theme_file($path) {
  $info = pathinfo($path); 
  $size = ' (' . format_size(filesize($path)) . ')';
  return l($info["basename"], "acquia_backups/".$info["basename"]) . $size;
}