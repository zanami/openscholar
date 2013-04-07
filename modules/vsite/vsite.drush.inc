<?php

/**
 * @file
 * Drush functionality file for the vsite module.
 */


/**
 * Implements hook_drush_command().
 */
function vsite_drush_command() {
  $items['vsite-redirect-disable'] = array(
    'description' => dt('Disable a vsite custom url.'),
    'arguments' => array(
      'nid' => dt('The nid of the vsite.'),
    ),
    'examples' => array(
      'drush vrd 1',
    ),
    'aliases' => array('vrd'),
  );
  return $items;
}

/**
 * Command callback.
 */
function drush_vsite_redirect_disable($nid) {
  db_delete('spaces_overrides')
    ->condition('id', $nid)
    ->condition('object_type', 'variable')
    ->condition('object_id', 'vsite_domain_name')
    ->execute();

  // Verify that the vsite don't have a custom path.
  $result = db_select('purl')
    ->fields('purl', array('value'))
    ->condition('provider', 'spaces_og')
    ->condition('id', $nid)
    ->execute()
    ->fetchAssoc();

  // Clearing the cache for the change to take place.
  drupal_flush_all_caches();

  $node = node_load($nid);
  $params = array(
    '@name' => $node->title,
    '@address' => variable_get('purl_base_domain') . '/' . $result['value'],
  );

  drush_log(dt('The custom url for @name has disabled. The address is @address', $params), 'success');
}
