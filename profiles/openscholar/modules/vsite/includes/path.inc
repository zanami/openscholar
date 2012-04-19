<?php

/**
 * Return the pathauto settings modified to work inside vsites
 */
function _vsite_get_pathauto_settings(){
  $pathauto_settings = array();
  
  $bundles = os_get_bundles();
  $entity = 'node';
  foreach ($bundles as $bundle) {
    $pattern = pathauto_pattern_load_by_entity($entity,$bundle);
    $pathauto_settings["pathauto_{$entity}_{$bundle}_pattern"] = "[vsite:site-purl]/".$pattern;
  }
  
  return $pathauto_settings;
}