<?php

/**
 * Defines functions to make using popups generally easier
 */

function os_popup_link($type, $text, $dest, $alt, $class) {
  if ($type == 'modalframe' && (!module_exists('modalframe') && !module_exists('os_modalframe'))) {
    $type = 'ctools';
  }
  if ($type == 'dialog' && !module_exists('dialog')) {
    $type = 'ctools';
  }
  $output = '';
  switch ($type) {
    case 'ctools':
      ctools_include('ajax');
      ctools_include('modal');

      $output = ctools_modal_text_button($text, $dest, $alt, $class);
    break;
    case 'dialog':
      ctools_include('ajax');

      $output = ctools_ajax_text_button($text, $dest, $alt, $class, 'ctools-use-dialog');
    break;
    case 'modalframe':
      ctools_include('ajax');
      drupal_add_js(drupal_get_path('module', 'os_modalframe').'/theme/os_modalframe_links.js');

      $output = ctools_ajax_text_button($text, $dest, $alt, $class, 'os-popup-modal-link');
    break;
    default:
      $output = l($text, $dest, array('class' => $class, 'title' => $alt));
    break;
  }

  return $output;
}