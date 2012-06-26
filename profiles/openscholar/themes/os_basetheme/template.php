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
  if ($GLOBALS['theme'] == 'cp_theme') return;
  $regions = array (
    'header-left' => $vars['page']['header_second'],
    'header-main' => $vars['page']['header_first'],
    'header-right' => $vars['page']['header_third'],
    'content-top' => $vars['page']['content_top'],
    'content-left' => $vars['page']['content_first'],
    'content-right' => $vars['page']['content_second'],
    'content-bottom' => $vars['page']['content_bottom'],
  );

  $non_empty_regions = array_filter($regions, "__os_basetheme_is_empty");
  $header_classes = '';
  if (count($non_empty_regions)) {
    $header_classes = implode(' ', array_keys($non_empty_regions));
   }
  else {
  }

  $vars['classes_array'][] = $header_classes;
}

/**
 * For header region classes
 */
function __os_basetheme_is_empty($s){
  return $s ? TRUE : FALSE;
}