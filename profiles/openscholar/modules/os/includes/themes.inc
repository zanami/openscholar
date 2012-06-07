<?php

/**
 * Theme api stuff. Get/set/list themes etc.
 */

/**
 * Returns HTML for the Powered by Openscholar text.
 *
 * @ingroup themeable
 */
function theme_openscholar_powered_by() {
  return '<span>' . t('Powered by <a href="@poweredby">OpenScholar</a>', array('@poweredby' => 'http://openscholar.harvard.edu')) . '</span>';
}

/**
 * Returns all OS themes for current privacy settings
 */
  
function os_get_themes($theme_type = 'vsite'){
  $allthemes = list_themes();
  _os_get_theme_type($theme_type);

  return (array_filter($allthemes, '_os_is_os_theme'));
}

/**
 * Filters keeps only OS themes.
 **/
function _os_is_os_theme($theme) {
  $space = spaces_get_space();
  $site_key = ($space) ? $space->group->purl : false;
  
  return (array_key_exists('os',$theme->info)
     && array_key_exists('theme_type', $theme->info['os'])
     && _os_get_theme_type() == $theme->info['os']['theme_type']
     && ($theme->info['os']['theme_access'] != "private" || user_access('access private scholar themes') || $site_key == strtolower($theme->name))
     && !empty($theme->status));
}

function _os_get_theme_type($set_type = false) {
  static $s_theme_type;
  if ($set_type) {
    $s_theme_type = $set_type;
  }
  return $s_theme_type;
}

/**
 * Gets flavors for theme
 **/
function os_theme_get_flavors($theme = NULL) {
	ctools_include('plugins');
  $flavors = ctools_get_plugins('cp_appearance', 'flavor');
    
  if(!$theme){
  	return $flavors;
  }

  //Get just the plugins for a specific theme
  static $theme_plugins;
  if(!isset($theme_plugins)){
  	foreach ($flavors as $key => $info) {
  	  $theme_plugins[$info['module']][$key] = $info;
  	}
  }

  if(!is_array($theme_plugins)) {
    return array();
  }

  return (array_key_exists($theme, $theme_plugins)) ? $theme_plugins[$theme] : array();
  
}