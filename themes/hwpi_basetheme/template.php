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



