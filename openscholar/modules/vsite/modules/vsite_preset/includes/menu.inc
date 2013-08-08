<?php

/**
 * @file
 * Menu and task functions that only need to be included in certain cases.
 */

/**
 * Task callback to create a News & Events page section and menu.
 */
function _vsite_preset_section_newsevents_task($entity, $task, &$context) {
  // @todo create required page nodes and menu items.
  $node = os_node_create(t('News & Events'), 'page', $entity->uid);
  vsite_add_entity($node, 'node', $entity);
}
