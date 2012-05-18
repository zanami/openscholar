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

    if(module_exists('vsite_menu')){
      $tree = vsite_menu_os_menu_tree($menu_name);
    }

    if(!$tree){
      $tree = menu_tree_page_data($menu_name);
    }

    //Allow other modules to make changes to the tree
    drupal_alter('os_menu_tree', $menu_name, $tree);

    $menu_output[$menu_name] = menu_tree_output($tree);
  }
  return $menu_output[$menu_name];
}

/**
 * Saves a menu link.
 *
 * @param $item
 *   An associative array representing a menu link item, with elements:
 *   - link_path: (required) The path of the menu item, which should be
 *     normalized first by calling drupal_get_normal_path() on it.
 *   - link_title: (required) Title to appear in menu for the link.
 *   - menu_name: (optional) The machine name of the menu for the link.
 *     Defaults to 'navigation'.
 *   - weight: (optional) Integer to determine position in menu. Default is 0.
 *   - expanded: (optional) Boolean that determines if the item is expanded.
 *   - options: (optional) An array of options, see l() for more.
 *   - mlid: (optional) Menu link identifier, the primary integer key for each
 *     menu link. Can be set to an existing value, or to 0 or NULL
 *     to insert a new link.
 *   - plid: (optional) The mlid of the parent.
 *   - router_path: (optional) The path of the relevant router item.
 *
 * @return
 *   The mlid of the saved menu link, or FALSE if the menu link could not be saved.
 */
function os_menu_link_save(&$item) {

  $mlid = false;
  if(module_exists('vsite_menu')){
    //Save a menu item using the vsite controllers
    $mlid = vsite_menu_menu_link_save($item);
  }else{
    //Call the standard drupal menu save (this will clear all menu caches)
    $mlid = menu_link_save($item);
  }

  return $mlid;
}

/**
 * Delete one menu link.
 * @param $menu
 *   A valid menu identifier
 * @param $mlid
 *   A valid menu link mlid
 */
function os_menu_link_delete($menu, $mlid) {

  if(module_exists('vsite_menu')){
    //Delete a menu item using the vsite controllers
    $mlid = vsite_menu_menu_link_delete($menu, $mlid);
  }else{
    //Call the standard drupal menu delete (this will clear all menu caches)
    _menu_delete_item(db_query("SELECT * FROM {menu_links} WHERE mlid = :mlid", array(':mlid' => $mlid))->fetchAssoc());
  }
}

/**
 * Load a menu link.
 *
 * @param $menu
 *   A valid menu identifier
 * @param $mlid
 *   A valid menu link mlid
 *
 * @return
 *   A menu link, with $item['access'] filled and link translated for
 *   rendering.
 */
function os_menu_link_load($menu, $mlid) {

  if(module_exists('vsite_menu')){
    //Return a menu item loaded from vsite menu controller
    return vsite_menu_menu_link_load($menu, $mlid);
  }else{
    //Call the standard drupal menu_link_load, _menu_translate is run to set access
    return menu_link_load($mlid);
  }
}

/**
 * Form for editing an entire menu tree at once.
 *
 * Shows for one menu the menu links accessible to the current user and relevant operations.
 */
function os_menu_overview_form($form, &$form_state, $menu) {

  //If the menu string was passed load the full menu
  if(is_string($menu)) $menu = menu_load($menu);

  if(module_exists('vsite_menu')){
    include_once drupal_get_path('module', 'vsite_menu') . '/vsite_menu.admin.inc';

    //Retrieve a menu item using the vsite controllers
    $form = vsite_menu_menu_overview_form($form, $form_state, $menu);
  }else{
    //Call the standard drupal menu retrieve
    $form = menu_overview_form($form, $form_state, $menu);
  }

  //Setup default submit and theme handlers
  $form['#submit'][] = 'menu_overview_form_submit';
  $form['#theme'][] = 'menu_overview_form';

  return $form;
}
