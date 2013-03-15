<?php

/**
 * @file
 * Drush script for converting private people pictures to public pictures.
 *
 * Script parameters:
 *   --fids
 *     File IDs to move from private to public.
 */

/**
 * Returning the destined path for the file.
 *
 *  @param $uri
 *    The original URI of the file.
 *
 *  @return
 *    Destined path for the file.
 */
function _os_files_deprivatize_get_dest_path($uri) {
  $exploded_path = explode('/', $uri);
  $i = count($exploded_path) - 1;
  unset($exploded_path[$i]);

  return implode('/', $exploded_path);
}

/**
 * Attempts to move a file from private:// to public://.
 *
 * @return bool
 *  - Returns TRUE if any modifications were made.
 */
function _os_files_deprivatize($fid, $uri = NULL) {
  ddd((int) $fid,'$fid',FALSE);

  if (!$uri) {
    $result = db_select('file_managed', 'f')
      ->fields('f', array('uri'))
      ->condition('fid', (int) $fid)
      ->execute()
      ->fetchAssoc();
    if (isset($result['uri'])) {
      drush_log("Success!!");
      $uri = $result['uri'];
      drush_log(dt('Found file @fid with URI @uri. Processing...', array('@fid' => $fid, '@uri' => $uri)));
    }
    else {
      drush_log(dt('Skipping @fid. File not found.', array('@fid' => $fid)));
      return FALSE;
    }
  }

  ddd($uri, '$uri', FALSE);

  if (strpos($uri, 'private://') !== 0) {
    drush_log(dt('Skipping @name. Not a private file.', array('@name' => $value['filename'])), 'warning');
    return FALSE;
  }
  drush_log("Attempting to move file at $uri");
  return _os_files_deprivatize_file($fid);
}

/**
 * Moves a private managed file to the public directory.
 */
function _os_files_deprivatize_file($fid) {
  $file = file_load($fid);

  // Builds new public path.
  $dest_uri = str_replace('private://', 'public://', $file->uri);
  $dest_path = _os_files_deprivatize_get_dest_path($dest_uri);
  // Creates the destination folder if it doesn't exist.
  if (!is_dir($dest_path)) {
    // Creates the folder.
    drupal_mkdir($dest_path, NULL, TRUE);
  }

  $moved_file = @file_move($file, $dest_uri);
  if ($moved_file) {
    drush_log(dt('File @name moved successfully.', array('@name' => $file->filename)), 'success');
  }

  $file->uri = $moved_file->uri;
  file_save($file);

  drush_log(dt('File @name updated successfully.', array('@name' => $file->filename)), 'success');
  return TRUE;
}

/**
 * Main function.
 */
function _os_files_privatize_main($fids) {
  $flush_cache = FALSE;

  // Loops over all file IDs and tries to modify each.
  $fids = explode(',', $fids);
  ddd($fids, '$fids', FALSE);
  foreach ($fids as $fid) {
    $modified = _os_files_deprivatize($fid);
    if (!$flush_cache && $modified) {
      $flush_cache = TRUE;
    }
  }

  // Flushes caches if any changes were made.
  if ($flush_cache) {
    drush_log(dt('Flushing the cache for update the files field data.'), 'warning');
    drupal_flush_all_caches();
  }
}

/**
 * DEBUG ONLY. REMOVE BEFORE MERGE.
 */
function ddd($value, $label, $boolean = TRUE) {
  if ($boolean) {
    $output = 'FALSE';
    if ($value) {
      $output = 'TRUE';
    }
  }
  else {
    $output = print_r($value, TRUE);
  }
  drush_log($label . ': ' . $output, 'warning');
}


// Runs main function.
$fids = drush_get_option('fids', 0);

_os_files_privatize_main($fids);