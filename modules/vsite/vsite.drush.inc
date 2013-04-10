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
    'options' => array(
      'nid' => dt('The nid of the vsite.'),
    ),
    'examples' => array(
      'drush vrd --nid=1' => dt('Delete the url for the vsite with the 1 identifier'),
      'drush vrd' => dt('Disable all of the custom URL.'),
    ),
    'aliases' => array('vrd'),
  );
  $items['vsite-redirect-list'] = array(
    'description' => dt('List custom vsite URLs.'),
    'options' => array(),
    'examples' => array('drush vrl'),
    'aliases' => array('vrl'),
  );
  return $items;
}

function drush_vsite_redirect_list() {
  $query = db_select('purl')
    ->fields('purl', array('value', 'id'))
    ->condition('provider', 'vsite_domain');
  $query->join('node', 'n', 'n.nid = purl.id');
  $query->fields('n', array('title'));
  $results = $query->execute();

  foreach ($results->fetchAllAssoc('id') as $result) {
    drush_print(dt('@id: @domain -> @title', array('@title'=>$result->title, '@id'=>$result->id, '@domain' => $result->value)));
  }
}

/**
 * Command callback.
 */
function drush_vsite_redirect_disable() {
  $nid = drush_get_option('nid', 0);

  $spaces_query = db_delete('spaces_overrides')
    ->condition('object_type', 'variable')
    ->condition('object_id', 'vsite_domain_name');

  $purl_query = db_delete('purl')
    ->condition('provider', 'vsite_domain');

  if ($nid) {
    $spaces_query->condition('id', $nid);
    $purl_query->condition('id', $nid);
  }

  $spaces_query->execute();
  $purl_query->execute(); 

  if ($nid) {
    // Get the vsite path.
    $result = db_select('purl')
      ->fields('purl', array('value'))
      ->condition('provider', 'spaces_og')
      ->condition('id', $nid)
      ->execute()
      ->fetchAssoc();
  }


  // Clearing the cache for the change to take place.
  drupal_flush_all_caches();

  if ($nid) {
    $node = node_load($nid);
    $params = array(
      '@name' => $node->title,
      '@address' => variable_get('purl_base_domain') . '/' . $result['value'],
    );
    drush_log(dt('The custom url for @name has disabled. The address is @address', $params), 'success');
  }
  else {
    drush_log(dt('All of the custom URLs were delete.'), 'success');
  }

}
