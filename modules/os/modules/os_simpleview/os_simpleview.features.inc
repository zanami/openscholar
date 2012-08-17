<?php

/**
 * Implementation of hook_views_api().
 */
function os_simpleview_views_api() {
  return array(
    'api' => '2',
  );
}


/**
 * Implementation of hook_ctools_plugin_api().
 */
function os_simpleview_ctools_plugin_api($module, $api) {
  if ($module == 'os_simpleview' && $api == 'plugins') {
    return array('version' => 1);
  } elseif ($module == 'boxes' && $api == 'plugins') {
    return array('version' => 1);
  }
}
