<?php
/**
 * Preprocess and Process Functions SEE: http://drupal.org/node/254940#variables-processor
 * 1. Rename each function and instance of "adaptivetheme_subtheme" to match
 *    your subthemes name, e.g. if your theme name is "footheme" then the function
 *    name will be "footheme_preprocess_hook". Tip - you can search/replace
 *    on "adaptivetheme_subtheme".
 * 2. Uncomment the required function to use.
 */

/**
 * // Adds classes to the body tag.
 */
function os_basetheme_preprocess_page(&$vars) {
  //Adds OpenScholar header region awareness to body classes
  $regions = array (
    'left' => $vars['three_col_first'],
    'main' => $vars['three_col_second'],
    'right' => $vars['three_col_third'],
  );

  $non_empty_regions = array_filter($regions, "__scholar_base_is_empty");
  $header_classes = 'header-';
  if (count($non_empty_regions)) {
    $header_classes .= implode('-', array_keys($non_empty_regions));
   }
  else {
    $header_classes .= 'none';
  }

  $vars['classes_array'][] = $header_classes;
}

