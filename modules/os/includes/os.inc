<?php
// $Id$

/**
 * @file os.inc
 * Provides generic OS management and feature functions. This file is included
 * by default.
 */

/**
 * Returns avalible bundles based on privacy.
 *
 * @param $privacy
 *  Can be array:
 *    - 0: disabled
 *    - 2: private
 *    - 1: public
 *  TRUE returns all
 *
 * @param $space_type
 * @param $reset
 *   (reset features cache)
 *
 * @return
 *  Array keyed with the bundle ID and the bundle name as value.
 */
function os_get_bundles($privacy = array(1), $space_type = 'og', $reset = FALSE) {
  $ret_all = FALSE;
  if (is_bool($privacy) && $privacy) {
    $ret_all = TRUE;
  }
  elseif (!is_array($privacy)) {
    $privacy = array($privacy);
  }

  $bundles = array();

  // Reset static feature list.
  $features = spaces_features($space_type, $reset);
  $site_features = variable_get('spaces_features', array());

  foreach ($features as $name => $feature) {
    $info = $feature->info['features'];
    if (!isset($info['node'])) {
      continue;
    }
    if (!$ret_all && (!isset($site_features[$name]) || !in_array($site_features[$name], $privacy))) {
      continue;
    }

    // Get bundle name, we call the node_info hook directly since
    // this is called during bootstrap
    module_load_include('features.inc', $name);
    $node_info_hook = $name . '_node_info';
    if (function_exists($node_info_hook)) {
      $node_info = $node_info_hook();
    }

    foreach ($info['node'] as $nodetype) {
      // Hide harvard API importer node from content types list.
      if ($nodetype == 'harvard_api_importer') {
        continue;
      }

      if (isset($node_info[$nodetype]['name'])) {
        $bundles[$nodetype] = $node_info[$nodetype]['name'];
      }
      else {
        $bundles[$nodetype] = $feature->info['name'];
      }
    }
  }

  return $bundles;
}

/**
 * Returns the current active feature based on active contexts.
 *
 * @return object
 * The active feature.
 */
function os_get_active_feature() {
  static $active_feature;

  if (!isset($active_feature)) {
    $features = spaces_features('og');
    $contexts = context_active_contexts();
    if (!empty($contexts) && is_array($contexts)) {
      foreach ($features as $feature) {
        $info = $feature->info['features'];
        if (isset($info['context'])) {
          foreach ($info['context'] as $ctx) {
            if (array_key_exists($ctx, $contexts)) {
              $active_feature = $feature;
              break (2);
            }
          }
        }
      }
    }
  }

  return $active_feature;
}

/**
 * Gets the base context path for a feature.  This is the path that will match
 * to this features context.
 *
 * @param string $app_name
 *   The machine name of the app (a feature module)
 *
 * @return string $path
 */
function os_get_app_path($app_name) {
  $app_info = os_app_info($app_name);

  if (isset($app_info['path'])) {
    $path = $app_info['path'];
    if (strlen($path) > 0) {
      return $path;
    }
  }

  return FALSE;
}

