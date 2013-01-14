<?php
function cp_content_add_page() {
  $item = menu_get_item('node/add');
  $content = system_admin_menu_block($item);
  // Bypass the node/add listing if only one content type is available.

  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }

  $bundles = os_get_bundles();

  $links = array();
  foreach ($bundles as $type => $name) {
    $url_str = str_replace('_', '-', $type);
    foreach ($content as $link) {
      if ($link['link_path'] == 'node/add/'.$url_str) {
        $links[] = $link;
      }
    }
  }

  return theme('node_add_list', array('content' => $links));
}

/**
 * Same as _node_add_access but just for os bundles
 * @return boolean
 */
function cp_content_add_page_access() {
  $bundles = os_get_bundles();
  foreach ($bundles as $type => $name) {
    if (node_hook($type, 'form') && node_access('create', $type)) {
      return TRUE;
    }
  }
  if (user_access('administer content types')) {
    // There are no content types defined that the user has permission to create,
    // but the user does have the permission to administer the content types, so
    // grant them access to the page anyway.
    return TRUE;
  }
  return FALSE;
}