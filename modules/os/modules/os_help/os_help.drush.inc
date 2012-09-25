<?php
// $Id$


/**
 * @file
 * Provides drush command `build_docs` for OpenScholar Advanced Help files.
 */

/**
 * Implements hook_drush_help().
 */
function os_help_drush_help($section) {
  /*
  if ((string) $section === 'drush:build_docs') {
    return dt('Build advanced help documentation from os_help files.');
  }
  */
}

/**
 * Implements hook_drush_command().
 */
function os_help_drush_command() {
  /*
  $commands = array(
    'build_docs' => array(
      'description' => 'Generate advanced help .ini files for boxes.',
      'options' => array(
        '--help-readme' => 'Displays the build_docs readme, which includes information on setting up help files for build_docs.',
      ),
      'examples' => array(
        'drush @site build_docs' => 'Scan OpenScholar source for os_help files and attach them to os_help.',
      ),
      'aliases' => array('bd'),
      'bootstraph' => 'DRUSH_BOOTSTRAPH_DRUPAL_DATABASE',
    ),
  );

  return $commands;
  */
}

/**
 * Implements drush_hook_command().
 *
 * Finds *.os_help.ini files, includes them in OS Help.
 */
function drush_os_help_build_docs() {
  /*
  // newline shortcut
  define('BR', "\n");

  // Prints README file to command line and exits early if option is selected.
  $readme = drush_get_option('help-readme');
  if ($readme) {
    echo BR . file_get_contents(drupal_get_path('module', 'os_help') . '/README.BUILD_DOCS.txt') . BR . BR;
    return;
  }

  // destination for ini and html files
  define('DEST', drupal_get_path('module', 'os_help') . '/help/');

  // create images dir if necessary.
  if (!is_dir(DEST . '/images/')) {
    mkdir(DEST . '/images', 0775);
  }

  // get all os_help files
  $files = drupal_system_listing('.*\.os_help\.ini$', 'modules');

  // start with the default template
  $ini = file_get_contents(drupal_get_path('module', 'os_help') . '/os_help.template.ini') . BR . BR;
  $done = array();

  // loop over all os_help.ini files
  foreach ($files as $name => $file) {
    $helpfile = str_replace('os_help', 'html', $name);
    $path = str_replace($file->basename, '', $file->filename);

    // append fragment of ini file
    $append = trim(file_get_contents($file->filename)) . BR;
    $append .= BR . BR;
    $ini .= $append;

    // copy html help file
    copy($path . $helpfile, DEST . $helpfile);

    // copy images folder
    foreach (file_scan_directory($path . '/images/', '.') as $i => $img) {
      copy($path . 'images/' . $img->basename, DEST . '/images/' . $img->basename);
    }

    $done[] = str_replace('.os_help', '', $file->name);
  }

  echo 'Included documentation for ' . implode(', ', $done);
  file_put_contents(DEST . '/os_help.help.ini', $ini);
  */
}

