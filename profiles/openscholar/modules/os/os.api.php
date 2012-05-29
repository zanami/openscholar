<?php

/**
 * @file
 * Hooks provided by OpenScholar.
 */

/**
 * Implementation of hook os_widget
 *
 * @returns array
 * Blocks that this module would like to be avalible in the openscholar layout
 */
function hook_os_widget(){
  
  //Block can make use of any field in the block schema
  

  return array(
      array(
          'title' => "My Modules block",
          'theme' => 'block_theme',
          'region' => 'sidebar_first',
          'weight' => 100,
          'hidden' => 0
      )
  );

}

/**
 * Implementation of hook os_widget_alter
 *
 * This function should add any parameters to the passed block that
 * will be needed in the admin user interfaces.  Including access params
 * config paths etc...
 *
 * @param $widget
 */
function hook_os_widget_alter(&$widget){
  
  //Does this widget belong to my module
  if ($widget->module == 'mymodule') {
    //Add a path to configure the widget that my module needs
    $widget['config_path'] = '/my/conf/path';
  }
}

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