<?php

/**
 * @file os.inc
 * Provides generic OS management and feature functions. This file is included by default.
 *
 * os_get_bundles
 * Return the avalible bundles based on the privacy.
 *
 * os_get_active_feature
 * Return the current active feature.
 *
 * os_get_feature_path
 * Get the base context path for a feature
 */

/**
 * Return the avalible bundles based on the privacy
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
    if (!isset($info['node'])) continue;
    if (!$ret_all && (!isset($site_features[$name]) || !in_array($site_features[$name], $privacy))) continue;

    // Get bundle name, we call the node_info hook directly since
    // this is called during bootstrap
    module_load_include('features.inc', $name);
    $node_info_hook = $name . '_node_info';
    if (function_exists($node_info_hook)) {
      $node_info = $node_info_hook();
    }

    foreach ($info['node'] as $nodetype) {
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
 * Return the current active feature based on active contexts
 *
 * @return object
 * The active feature.
 */
function os_get_active_feature(){
  static $active_feature;

  if(!isset($active_feature)){
    $features = spaces_features('og');
    $contexts = context_active_contexts();
    if(!empty($contexts) && is_array($contexts)){
      foreach ($features as $feature) {
        $info = $feature->info['features'];
        if (isset($info['context'])) {
          foreach ($info['context'] as $ctx) {
            if(array_key_exists($ctx, $contexts)){
              $active_feature = $feature;
              break(2);
            }
          }
        }
      }
    }
  }

  return $active_feature;
}

/**
 * Get the base context path for a feature.  This is the path that will match to this features context.
 *
 * @param string Feature
 *
 * @return string path
 */
function os_get_feature_path($feature){

  $feature = features_get_features($feature);
  if($feature && isset($feature->info['features']['context'])){
    foreach ($feature->info['features']['context'] as $context) {
    	$f_context = context_load($context);
    	if(isset($f_context->conditions['path']) && isset($f_context->conditions['path']['values'])){
    	  foreach ($f_context->conditions['path']['values'] as $path) {
    	  	if(substr($path,-2) == '/*'){
    	  	  return substr($path, 0, (strlen($path) - 2));
    	  	}
    	  }
    	}
    }

  }

  return false;
}