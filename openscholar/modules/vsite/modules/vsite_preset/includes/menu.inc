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
  vsite_add_entity($node, 'node', $entity);

  // Adds the primary menu link for the section page node.
  vsite_menu_add_node($node, $entity);
}
