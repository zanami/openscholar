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

/**
 * Implementation of hook os_layout_contexts
 *
 * Return Contexts along with thier label that your module creates
 * and would like to allow users to edit
 *
 * @return array
 * Avalible contexts with thier descriptions
 */
function hook_os_layout_contexts(){
  
  //Contexts provided by this module
  $provided_contexts = array(
      'my_context'=> 'Calendar Section',
      'another_context' => 'Twitter Page',
  );
  
  return $provided_contexts;
}

/**
 * Implementation of hook os_layout_contexts_alter
 *
 * Modify the contexts that are avalible for a user to edit
 */
function hook_os_layout_contexts_alter(&$all_contexts){
  
  // I do not want people to be able to edit this context when specail module is not
  // enabled so I am removing it from the list
  if(isset($all_contexts['special_context']) && !module_exists('special_module')){
    unset($all_contexts['special_context']);
  }
  
}