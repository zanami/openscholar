<?php
/**
 * @file
 * os.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function os_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-secondary-menu
  $menus['menu-secondary-menu'] = array(
    'menu_name' => 'menu-secondary-menu',
    'title' => 'Secondary Menu',
    'description' => 'A menu used on many sites to show secondary content.',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('A menu used on many sites to show secondary content.');
  t('Secondary Menu');


  return $menus;
}
