<?php

/**
 * Themable function to generate message after user submits
 *  cite_distribute widget selections
 *
 * @param array $modules
 *   List of repositories to which the node was submitted
 * @return string
 *   Returns textual list of the repositories titles.
 */
function theme_citation_distribute_form_msg($variables){
  $plugins = _citation_distribute_plugins();
  $modules = $variables['modules'];

  if ($modules) {
    $out = array();
    foreach ($modules as $module) {
        $out[] = $plugins[$module]['title'];
    }
    return 'Submitted citation to: ' . implode(', ', $out);
  } else {
      return false;
  }
}

/**
 * @function theme_citation_distribute_plugin_theme
 *
 * Theme function wrapper activates plugin and calls its theme function.
 */
function theme_citation_distribute_plugin_theme($variables) {
  $plugin = $variables['plugin'];
  $service = _citation_distribute_load_plugin($plugin);
  if ($service && method_exists($service, 'render')) {
    return $service->render($variables);
  }
}