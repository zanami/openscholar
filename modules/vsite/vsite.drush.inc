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
      'nid' => dt('The NID of the vsite.'),
    ),
    'alias' => array('vrd'),
  );
  return $items;
}


/**
 * Command callback.
 */
function drush_vsite_redirect_disable() {

}
