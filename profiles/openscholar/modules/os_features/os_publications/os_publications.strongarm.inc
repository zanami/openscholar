<?php
/**
 * @file
 * os_publications.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function os_publications_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'biblio_base';
  $strongarm->value = 'publications';
  $export['biblio_base'] = $strongarm;

  return $export;
}
