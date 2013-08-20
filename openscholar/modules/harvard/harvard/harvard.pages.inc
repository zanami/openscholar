<?php


/**
 * @function harvard_acquia_backups_page
 *
 * Makes our backups available to certain users
 */
function harvard_acquia_backups_page($filename = NULL) {
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

    if ($file) {
      drupal_set_message(t('Could not find file: @filename', array('@filename' => $filename)), 'error');
    }

    $list_items = array_map('_harvard_acquia_backups_page_theme_file', $files);
    return theme('item_list', array('items' => $list_items, 'type' => 'ul'));
  }
}


/**
 * @function _harvard_acquia_backups_page_theme_file
 *
 * Returns formatted version of filename
 */
function _harvard_acquia_backups_page_theme_file($path) {
  $info = pathinfo($path);
  $size = ' (' . format_size(filesize($path)) . ')';
  return l($info["basename"], "acquia_backups/".$info["basename"]) . $size;
}