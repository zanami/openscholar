<?php

/**
 * @file
 * Menu and task functions that only need to be included in certain cases.
 */

/**
 * Task callback to create a News & Events page section and menu.
 */
function _vsite_preset_section_newsevents_task($entity, $task, &$context) {
  // Adds the top-level section page node.
  $node = os_create_node(t('News & Events'), 'page', $entity->uid);
  vsite_add_node($node, $entity);

  // Adds the primary menu link for the section page node.
  $mlid = vsite_menu_add_node($node->nid, $entity, $node->title);
}

/**
 * Task callback to create an About page section and menu.
 */
function _vsite_preset_section_about_task($entity, $task, &$context) {
  // Adds the top-level section page node.
  $node = os_create_node(t('About'), 'page', $entity->uid);
  vsite_add_node($node, $entity->nid);

  // Adds the primary menu link for the section page node.
  $mlid = vsite_menu_add_node($node->nid, $entity->nid, $node->title);
}

/**
 * Task callback to create an About page section and menu.
 */
function _vsite_preset_section_home_task($entity, $task, &$context) {
  // Adds the top-level section menu link to existing front page.
  $mlid = vsite_menu_create_menu_link('HOME', 'home', $entity->nid);
}

/**
 * Task callback to create an Academics page section and menu.
 */
function _vsite_preset_section_academics_task($entity, $task, &$context) {
  // Adds the top-level section page node.
  $node = os_create_node(t('Academics'), 'page', $entity->uid);
  vsite_add_node($node, $entity->nid);

  // Adds the primary menu link for the section page node.
  $mlid = vsite_menu_add_node($node->nid, $entity->nid, $node->title);
}
