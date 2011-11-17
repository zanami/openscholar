<?php 

/**
 * @file OS drush - collection of drush commands for openscholar
 */

function os_drush_help($section) {
  switch ($section){
    case 'drush:help_indexer':
      return dt('Generate advanced help .ini files.');
  }
}

// more options: find missing, regenerate all, dry run
function os_drush_command() {
  $cmds = array(
    'help_indexer' => array(
      'description' => 'Generate advanced help .ini files.',
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

function drush_os_help_indexer() {
  $done = array();
  foreach (module_list() as $module) {
    if (_is_box($module)) {
      $done[] = _mk_index($module);
    }
  }
}

function _mk_index($module) {
  $mod_path = drupal_get_path('module', $module);
  $inifile  = $mod_path . '/help/' . $module . '.ini';
  $helpfile = $mod_path . '/' . $module . '.html';
  
  if (file_exists($helpfile)) {
    mkdir($mod_path . '/help');
    $ini  = "[$module]\n";
    $ini .= "parent = foo\n";
    $ini .= "weight = \n"; 
    $ini .= 'title = ' . _box_title($module) . "\n";    

EOF;
    file_put_contents($inifile, $ini);  
  }
}

function _is_box($module) {
  return (function_exists($module . '_boxes_plugins'));
}

function _box_title($module) {
  require_once(drupal_get_path('module', $module) . '/' . $module . '.module');
  $func = $module . '_boxes_plugins';
  $def = array_pop( $func() );
  return $def['title'];
}