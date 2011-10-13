<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function os_modalframe_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "boxes" && $api == "box") {
    return array("version" => 1);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function os_modalframe_node_info() {
  $items = array(
    'free_image' => array(
      'name' => t('Free Image'),
      'module' => 'features',
      'description' => t('An Image not attached to a gallery.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
