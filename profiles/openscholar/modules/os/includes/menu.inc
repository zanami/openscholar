<?php

/**
 * Return the list of menu's that are associated with openscholar
 */
function os_get_menus(){
	
	$menus = variable_get('os_menus',array(
    'features' => 'Primary Menu',
    'scholar_secondary_menu' => 'Secondary Menu',
  ));
	
  drupal_alter('os_menus',$menus);
  
  return $menus;
  
}

/**
 * Return an individual openscholar menu object
 */
function os_menu_tree($menu_name) {
	
  $menu_output = &drupal_static(__FUNCTION__, array());

  $menus = os_get_menus();
  if(!isset($menus[$data['menu_key']])) return;
    
  if (!isset($menu_output[$menu_name])) {
  	
    $tree = menu_tree_page_data($menu_name);
    //Allow other modules to make changes to the tree
    drupal_alter('os_menu_tree',$menu_key, $tree);
    
    $menu_output[$menu_name] = menu_tree_output($tree);
  }
  return $menu_output[$menu_name];
}
