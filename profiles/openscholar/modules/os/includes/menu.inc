<?php

/**
 * Return the list of menu's that are associated with openscholar
 */
function os_get_menus(){
  
  $menus = variable_get('os_menus', array(
      'primary-menu' => 'Primary Menu',
      'secondary-menu' => 'Secondary Menu'
  ));
  
  drupal_alter('os_menus', $menus);
  
  return $menus;

}

/**
 * Return an individual openscholar menu object
 *
 * Replaces 'menu_tree'
 */
function os_menu_tree($menu_name){
  
  $menu_output = &drupal_static(__FUNCTION__, array());
  
  $menus = os_get_menus();
  if (! isset($menus[$menu_name]))
    return;
  
  if (! isset($menu_output[$menu_name])) {
    
    $tree = menu_tree_page_data($menu_name);
    //Allow other modules to make changes to the tree
    drupal_alter('os_menu_tree', $menu_name, $tree);
    
    $menu_output[$menu_name] = menu_tree_output($tree);
  }
  return $menu_output[$menu_name];
}

///**
// * Alter the menu blocks to use os_menu_tree if they are os_menus
// * @param array $block_info
// */
//function _os_menu_context_block_info_alter(&$block_info){
//	$os_menus = os_get_menus();
//
//	foreach ($os_menus as $menu_key => $name) {
//		$menu_key = str_replace('_','-',$menu_key);
//		if(isset($block_info['menu-'.$menu_key])){
//			$block_info['menu-'.$menu_key]['module'] = 'os';
//		}
//	}
//}