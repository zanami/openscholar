<?php

// Rebuild the theme data.
system_rebuild_theme_data();
drupal_theme_rebuild();


/**
 * Preprocess variables for html.tpl.php
 */
function hwpi_basetheme_preprocess_html(&$vars) {

  $scripts = array(
    'matchMedia.js', // matchMedia polyfill for older browsers
    'eq.js',         // Equalheights script
    'eq-os.js',      // Call Equalheights for specific elements
  );
  foreach ($scripts as $script) {
    // See load.inc in AT Core, load_subtheme_script() is a wrapper for drupal_add_js()
    load_subtheme_script('js/' . $script, 'hwpi_basetheme', 'header', $weight = NULL);
  }

  // debug message
  //$message = 'Blog entry Blog Post Elements Cheat Sheet has been updated.';
  //drupal_set_message($message, 'status');
  //drupal_set_message($message, 'warning');
  //drupal_set_message($message, 'error');
}


/**
 * Preprocess variables for comment.tpl.php
 */
function hwpi_basetheme_preprocess_node(&$vars) {
  //dsm($vars);

  if($vars['node']->type == 'event') {
    $vars['event_start'] = array();
    if(isset($vars['field_date'][0]['value']) && !empty($vars['field_date'][0]['value'])) {
      date_default_timezone_set($vars['field_date'][0]['timezone']);
      $event_start_date = strtotime($vars['field_date'][0]['value']);
      $vars['event_start']['month'] = check_plain(date('M', $event_start_date));
      $vars['event_start']['day'] = check_plain(date('d', $event_start_date));
      $vars['classes_array'][] = 'event-start';
    }
  }
}


/**
 * Preprocess variables for comment.tpl.php
 */
function hwpi_basetheme_preprocess_comment(&$vars) {
  if($vars['new']) {
    $vars['title'] = $vars['title'] . '<em class="new">' . $vars['new'] . '</em>';
    $vars['new'] = '';
  }
}




/**
 * Returns HTML for a menu link and submenu.
 *
 * Adaptivetheme overrides this to insert extra classes including a depth
 * class and a menu id class. It can also wrap menu items in span elements.
 *
 * @param $vars
 *   An associative array containing:
 *   - element: Structured array data for a menu link.
 */
function hwpi_basetheme_menu_link(array $vars) {
  $element = $vars['element'];
  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  if (!empty($element['#original_link'])) {
    if (!empty($element['#original_link']['depth'])) {
      $element['#attributes']['class'][] = 'menu-depth-' . $element['#original_link']['depth'];
    }
    if (!empty($element['#original_link']['mlid'])) {
      $element['#attributes']['class'][] = 'menu-item-' . $element['#original_link']['mlid'];
    }
  }

  if (isset($element['#localized_options']) && !empty($element['#localized_options']['attributes']['title'])) {
    unset($element['#localized_options']['attributes']['title']);
  }

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>";
}
