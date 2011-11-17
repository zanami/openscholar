<?php 

/**
 * @file OS drush - collection of drush commands for openscholar
 */

/**
 * implementation of hook_drush_help
 */
function os_drush_help($section) {
  switch ($section){
    case 'drush:help_indexer':
      return dt('Generate advanced help .ini files for boxes.');
  }
}

/**
 * implementation of hook_drush_command
 */
// more options: find missing, regenerate all, dry run
//arg for specifying one module
function os_drush_command() {
  $cmds = array(
    'help_indexer' => array(
      'description' => 'Generate advanced help .ini files for boxes.',
      'options' => array(
        'force' => 'Force creation of index over an existing index file (default=false).',
        'module' => 'Comma separated list of modules to index.',
        'listonly' => 'List the modules that would receive an index, but don\'t make any changes.',
       ),
      'examples' => array(
        'drush @site help_indexer' => 'Create a help index for all boxes with help files but no index.',
        'drush @site hi --force --module=os_mailchimp,vsite' => 'Overwrite exisiting help index, but only for os_mailchimp and vsite modules.',
        'drush @site hi --listonly' => 'Show a list of the modules whose indices would be updated if run without --listonly.',
      ),
      'aliases' => array('hi'),
      'bootstraph' => 'DRUSH_BOOTSTRAPH_DRUPAL_DATABASE',
    ),
  );
  
  return $cmds;
}

/**
 * drush command help_indexer
 * finds modules that define boxes.  creates .ini file advertising their help files.
 */
function drush_os_help_indexer() {
  $opt = new stdClass();
  $opt->force = drush_get_option( array('force','f'), false );
  $opt->module = drush_get_option( array('module','mod') );
  $opt->listonly = drush_get_option( array('listonly', 'list'), false);
  
  $modules = (isset($opt->module)) ? explode(',',$opt->module) : module_list(); 
 
  $done = array();
  foreach ($modules as $module) {
    if (_is_box($module)) {
      $done[] = _mk_index($module, $opt);  
    }
  }
  
  $done = array_filter($done);
  $msg = ($opt->listonly) ? 'The following modules have orphaned help files: ' : 'Created help index for: ';
  if (count($done)>0) {
    echo $msg . implode(', ', $done) . "\n";
  } else {
    echo "Couldn't find orphaned help files.\n";
  }
}


/**
 * @function _mk_index($module)
 * given a module, creates advanced help for its boxes
 * @return name of module or false
 */
function _mk_index($module, &$opt) {
  $mod_path = drupal_get_path('module', $module);
  $inifile  = $mod_path . '/help/' . $module . '.help.ini';
  if (file_exists($inifile) && !$opt->force) {
    return FALSE; //don't overwrite a good ini file
  }
  
  require_once($mod_path . '/' . $module . '.module');
  $func = $module . '_boxes_plugins';
  $plugins = $func();
  
  $txt = "[advanced help settings]\nhide = TRUE \n\n";  //help topics are moved to os_boxes, so hide this module's entry
  $write = FALSE; //don't write a file unless we've found valid help files
  
  foreach($plugins as $plugin => $def) {
    if ( file_exists($mod_path . '/help/' . $plugin . '.html') || (isset($def['help']) && file_exists($mod_path . '/' . $def['help'])) ) {
      $write = TRUE; 
      $txt .= "[$plugin] \n";
      $txt .= "file = $plugin \n";
      $txt .= "parent = os_boxes% \n";
      $txt .= "weight = 0 \n";
      $txt .= 'title = ' . $def['title'] . "\n\n";
    }
  }
  
  if ($write) {
    if ($opt->listonly || file_put_contents($inifile, $txt)) { return $module; }
  } else {
    return FALSE;
  }
  
}

function _is_box($module) {
  return (function_exists($module . '_boxes_plugins'));
}

