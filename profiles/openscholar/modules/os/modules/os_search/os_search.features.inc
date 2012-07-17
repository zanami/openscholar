<?php
/**
 * @file
 * os_search.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function os_search_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "apachesolr_search" && $api == "apachesolr_search_defaults") {
    return array("version" => "3");
  }
}
