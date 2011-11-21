<?php 

/**
 * @file vsitehelp.drush.inc - collection of drush commands for openscholar
 */

/**
 * implementation of hook_drush_help
 */
function vsitehelp_drush_help($section) {
  switch ($section){
    case 'drush:build_docs':
      return dt('Generate advanced help .ini files for boxes.');
  }
}

/**
 * implementation of hook_drush_command
 */
// more options: find missing, regenerate all, dry run
//arg for specifying one module
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
 * drush command help_indexer
 * finds modules that define boxes.  creates .ini file advertising their help files.
 */
function drush_vsitehelp_build_docs() {
  define( 'DIR', '/tmp/vsitehelp/' );  //destination for ini and html files
  define( 'BR', "\n");  //newline shortcut
  
  
  // get all os_help files
  $files = drupal_system_listing('.*\.os_help\.ini$', 'modules');
  
  
  //and all includes as well
  //format is module => parent override
  /*$includes = array(
    'features' => 'developer',
  );
  foreach (drupal_system_listing('.*\.help\.ini$', 'module') as $filename => $obj) {
    
  }*/
  //maybe just listing additional ini files and their parent will work better
  
  // start with the default template
  $ini = file_get_contents(drupal_get_path('module', 'vsitehelp') . '/vsitehelp.template.ini') . BR . BR;
  
  foreach($files as $name => $o_file) {
    $helpfile = str_replace('os_help', 'html', $name);
    $path = str_replace($o_file->basename, '', $o_file->filename);
    
    $ini .= file_get_contents($o_file->filename) . BR;
    copy($path.$helpfile, DIR.$helpfile);
  }
  echo '<pre>' . $ini . '</pre>';
  
  file_put_contents(DIR . '/vsitehelp.ini', $ini);
/*  $opt = new stdClass();
  $opt->force = drush_get_option( array('force','f'), false );
  $opt->module = drush_get_option( array('module','mod') );
  $opt->listonly = drush_get_option( array('listonly', 'list'), false);
  */
}

