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

  $modified = _vsite_preset_menu_order($order, $entity->nid, 'primary-menu');
  return $modified;
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
  if (empty($menu) || empty($order)) {
    return FALSE;
  }

  // Assumes no modifications.
  $modified = FALSE;
  // By default, all newly created menu items will have weight 0.
  // Starts our menu weights at -1 in order to be at the beginning.
  $weight = -1;
  // Reverses order array to "count backwards" from the front-most menu link weight.
  $order = array_reverse($order);
  foreach ($order as $title) {
    // Loops through the existing menu items to search for this title.
    foreach ($menu as $mlid => $item) {
      // Skips (doesn't order) child menu items; only orders top-level links.
      if (isset($item['depth']) && $item['depth'] > 1) {
        continue;
      }
      // If the title matches...
      if ($item['link_title'] == $title) {
        // Modify the weight.
        $menu[$mlid]['weight'] = $weight;
        $weight = $weight - 1;
        $modified = TRUE;
        // Goes on to next specified title; skips over remaining items.
        break;
      }
    }
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
