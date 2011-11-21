<?php 

/**
 * @file vsitehelp.drush.inc 
 */

/**
 * implementation of hook_drush_help
 */
function vsitehelp_drush_help($section) {
  switch ($section){
    case 'drush:build_docs':
      return dt('Build advanced help documentation from os_help files.');
  }
}

/**
 * implementation of hook_drush_command
 */
function vsitehelp_drush_command() {
  $cmds = array(
    'build_docs' => array(
      'description' => 'Generate advanced help .ini files for boxes.',
      'options' => array(
        'force' => 'Force creation of index over an existing index file (default=false).',
        'module' => 'Comma separated list of modules to index.',
        'listonly' => 'List the modules that would receive an index, but don\'t make any changes.',
       ),
      'examples' => array(
        'drush @site help_indexer' => 'Create a help index for all boxes with help files but no index.',
        'drush @site hi --force --module=vsitehelp_mailchimp,vsite' => 'Overwrite exisiting help index, but only for vsitehelp_mailchimp and vsite modules.',
        'drush @site hi --listonly' => 'Show a list of the modules whose indices would be updated if run without --listonly.',
      ),
      'aliases' => array('bd'),
      'bootstraph' => 'DRUSH_BOOTSTRAPH_DRUPAL_DATABASE',
    ),
  );
  
  return $cmds;
}

/**
 * implementation of drush_hook_command
 * finds os_help files, includes them in vsitehelp
 */
function drush_vsitehelp_build_docs() {
  define( 'DIR', drupal_get_path('module', 'vsitehelp') . '/help/'  );  //destination for ini and html files
  define( 'BR', "\n");                                                  //newline shortcut
  
  // get all os_help files
  $files = drupal_system_listing('.*\.os_help\.ini$', 'modules');
    
  // start with the default template
  $ini = file_get_contents(drupal_get_path('module', 'vsitehelp') . '/vsitehelp.template.ini') . BR . BR;
  $done = array();
  
  foreach($files as $name => $o_file) {
    $helpfile = str_replace('os_help', 'html', $name);
    $path = str_replace($o_file->basename, '', $o_file->filename);
    
    $file = trim(file_get_contents($o_file->filename)) . BR;
    $file .= BR.BR;    
    $ini .= $file;

    copy($path.$helpfile, DIR . $helpfile);
    $done[] = str_replace('.os_help', '', $o_file->name);
  }
  
  echo 'Included documentation for ' . implode(', ', $done);
  file_put_contents(DIR . '/vsitehelp.help.ini', $ini);
}

