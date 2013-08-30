<?php

/**
 * @file
 * Menu and task functions that only need to be included in certain cases.
 */

/**
 * A generic task which creates a simple page node and primary menu link.
 */
function _vsite_preset_section_task($entity, $task, &$context) {
  // Only continues if a non-empty $title argument was passed on the task object.
  if (!isset($task->arguments['title']) || empty($task->arguments['title'])) {
    return;
  }

  // Creates a new page node using the passed title.
  $node = os_create_node($task->arguments['title'], 'page', $entity->uid);
  // Adds the new page node to the vsite this task is associated with.
  vsite_add_node($node, $entity->nid);

  // Optionally creates and adds additional pages as sub-pages (book child nodes).
  if (isset($task->arguments['info']['subpages'])) {
    $subpages = array();
    $options = array('parent_node' => $node->nid);
    foreach ($task->arguments['info']['subpages'] as $title) {
      $subpage = os_create_node($title, 'page', $entity->uid, NULL, $options);
      vsite_add_node($subpage, $entity->nid);
      $subpages[] = $subpage;
    }
  }

  // Creates a new primary menu link for the new page node.
  $mlid = vsite_menu_add_node($node->nid, $entity->nid, $node->title);
}

/**
 * A generic task which attempts to order existing menu items.
 */
function _vsite_preset_menu_order_task($entity, $task, &$context) {
  // Loads the preset menu order from the preset's variable.
  $preset = spaces_preset_load($entity->spaces_preset_og, 'og');
  if (isset($preset->value['variable']['vsite_preset_menu_order'])) {
    $order = $preset->value['variable']['vsite_preset_menu_order'];
  }
  else {
    return;
  }
  $modified = vsite_menu_order_links($order, $entity->nid, 'primary-menu');
}

/**
 * Attempts to order menu links to the front of the given menu.
 *
 * @param array $order
 * @param int $gid
 * @param string $menu_name
 *
 * @return bool $modified
 *   Whether or not the vsite's menu object was modified.
 */
function _vsite_preset_menu_order($order, $gid, $menu_name = 'primary-menu') {
  $vsite = vsite_get_vsite($gid);
  if (!$vsite) {
    return FALSE;
  }
  $menu = _vsite_menu_get_menu_links($menu_name, $vsite);
  if (empty($menu) || empty($links)) {
    return FALSE;
  }

  // Assume no modifications.
  $modified = FALSE;
  // @todo find the smallest weight in the existing menu links.
  $weight = 0;
  foreach ($order as $title) {
    // 1. Search for an existing menu link matching the title.
    // 2. If any is found, act on the first one:
    //  - modify the link weight
    //  - increment $weight
    //  - $modified = TRUE
  }

  if ($modified) {
    $vsite->controllers->menus->set($menu_name, $menu);
  }

  return $modified;
}

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
