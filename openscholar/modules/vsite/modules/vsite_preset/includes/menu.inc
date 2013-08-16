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
function _vsite_preset_section_home_task($entity, $task, &$context) {
  // Adds the top-level section menu link to existing front page.
  $mlid = vsite_menu_create_menu_link('HOME', 'home', $entity->nid);
}
