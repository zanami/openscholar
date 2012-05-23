<?php

/**
 * @file
 * Hooks provided by OpenScholar.
 */

/**
 * Alter the list of menu's that avalible in the openscholar UI
 *
 * The Menus array is strictured array(menu-id => 'Menu Name')
 */
function hook_os_menus_alter(&$menus){
  
  //Remove this menu
  if (isset($menus['primary-menu'])) {
    unset($menus['primary-menu']);
  }

}

/**
 * Alter openscholars menu_tree page data before it is passed to theme functions
 */
function hook_os_menu_tree_alter($menu_name, &$tree){
  
  foreach ( $tree as $id => $menu_link ) {
    if (count($menu_link['below'])) {
      //This menu link has children add somthing
      $tree[$id]['link']['opetions'] = 'xx';
    }
  }

}