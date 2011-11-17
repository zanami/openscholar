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
      'options' => array(),
      'examples' => array(
        'drush @site help_indexer',
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
  $done = array();
  foreach (module_list() as $module) {
    if (_is_box($module)) {
      $done[] = _mk_index($module);
    }
  }
  
  $done = array_filter($done);
  if (count($done)>0) {
    echo "Created help index for " . implode(', ', $done) . "\n";
  } else {
    echo "Couldn't find orphaned help files.\n";
  }
}


/**
 * @function _mk_index($module)
 * given a module, creates advanced help for its boxes
 * @return name of module or false
 */
function _mk_index($module) {
  $mod_path = drupal_get_path('module', $module);
  $inifile  = $mod_path . '/help/' . $module . '.help.ini';
  if (file_exists($inifile)) {
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
  
  if ($write && file_put_contents($inifile, $txt)) {
    return $module;
  } else {
    return FALSE;
  }
  
}

function _is_box($module) {
  return (function_exists($module . '_boxes_plugins'));
}

