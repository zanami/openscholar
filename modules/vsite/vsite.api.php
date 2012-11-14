<?php
// $Id$

/**
 * @file
 * vsite module API file.
 */

/**
 * Implements hook_vsite_app_info().
 *
 * Notifies vsite module about OS App configuration.
 *
 * @return array $info
 *   An associative array containing:
 *   - group: (optiona) An array of OG group node types (site types).
 *   - group content: (optional) An array of arrays, keyed by group content
 *     node type machine name.
 *     - NODETYPE: A string machine name matching hook_node_info().
 *       - singluar: A string, translated, human readable name for node type.
 *       - plural: A string, translated, human readable plural for node type.
 *   - restricted groups: (optional) An array of site types that should
 *     not see this app listed.
 *   - main path: (optional) A string, the app's main path menu item.
 *   - settings path: (optional) A string, the app's settings form path.
 */
function hook_vsite_app_info() {
  $info = array();

  // Exported app info for OS News.
  $info['os_news'] = array(
    'group' => array(),
    'group content' => array(
      'news' => array(
        'singular' => t('news item'),
        'plural' => t('news items'),
      ),
    ),
    'restricted groups' => array('project', 'department'),
    'main path' => 'news',
    'settings path' => 'features/os_news/settings',
  );

  return $info;
}

