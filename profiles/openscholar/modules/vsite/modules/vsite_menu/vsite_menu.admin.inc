<?php

/**
 * Submit handler for the menu overview form.
 * Replaces 'menu_overview_form_submit' inside a vsite
 *
 * @see menu_overview_form_submit()
 * @see menu_overview_form()
 */
function vsite_menu_menu_overview_form_submit($form, &$form_state){
  ctools_include('menu', 'os');
  $vsite = spaces_get_space();
  $os_menus = os_get_menus();
  $menu_name = $form['#menu']['menu_name'];
  
  //Only execute this function if we are in a space and this is an os_menu
  if (! $vsite || ! isset($vsite->controllers->menus) || ! array_key_exists($menu_name, $os_menus))
    return menu_overview_form_submit($form, $form_state);
    
  // When dealing with saving menu items, the order in which these items are
  // saved is critical. If a changed child item is saved before its parent,
  // the child item could be saved with an invalid path past its immediate
  // parent. To prevent this, save items in the form in the same order they
  // are sent by $_POST, ensuring parents are saved first, then their children.
  // See http://drupal.org/node/181126#comment-632270
  $order = array_flip(array_keys($form_state['input'])); // Get the $_POST order.
  $form = array_merge($order, $form); // Update our original form with the new order.
  

  $menu_links_schema = drupal_get_schema('menu_links');
  
  $items = array();
  $fields = array(
      'weight',
      'plid'
  );
  
  //Build the full menu_item list and mark the customized menu entries as such
  foreach ( element_children($form) as $mlid ) {
    if (isset($form[$mlid]['#item'])) {
      $element = $form[$mlid];
      // Update any fields that have changed in this menu item.
      foreach ( $fields as $field ) {
        if ($element[$field]['#value'] != $element[$field]['#default_value']) {
          $element['#item'][$field] = $element[$field]['#value'];
          $items[$mlid] = $element['#item'];
          $element['#item']['customized'] = 1;
        }
      }
      // Hidden is a special case, the value needs to be reversed.
      if ($element['hidden']['#value'] != $element['hidden']['#default_value']) {
        // Convert to integer rather than boolean due to PDO cast to string.
        $element['#item']['hidden'] = $element['hidden']['#value'] ? 0 : 1;
        $element['#item']['customized'] = 1;
        $items[$mlid] = $element['#item'];
      }
      
      if (! isset($items[$mlid]))
        $items[$mlid] = $element['#item'];
        
      //Clean up the Item for saving
      $items[$mlid] = array_intersect_key($items[$mlid], $menu_links_schema['fields']);
    }
  }
  
  $vsite->controllers->menus->set($menu_name, $items);
  drupal_set_message(t('Your configuration has been saved.'));
}


