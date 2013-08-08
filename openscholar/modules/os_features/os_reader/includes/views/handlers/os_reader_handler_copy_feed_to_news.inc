<?php

/**
 * @file
 * Views handler for displaying link that creates news node based on feed item.
 */

class os_reader_handler_copy_feed_to_news extends views_handler_field {

  function render($values) {
    global $user;
    if (!$user->uid) {
      return;
    }

    $groups = og_get_entity_groups();
    if (empty($groups['node'])) {
      return;
    }

    $vsite_ids = array_keys($groups['node']);
    $vsites = node_load_multiple($vsite_ids);

    $items = array();
    foreach ($vsites as $vsite) {
      $copy = t('Copy to @vsite', array('@vsite' => $vsite->title));
      if (os_reader_feed_orphan_from_node($values->id, $vsite->nid)) {
        $items[] = $copy;
      }
      else {
        $options = array(
          'attributes' => array(
            'class' => array('use-ajax'),
          ),
        );
        $url = l($copy, 'os-reader/copy-feed-to-news/' . $values->id . '/' . $vsite->nid . '/nojs', $options);
        $items[] = "<span id='feed-{$values->id}-{$vsite->nid}'>$url</span>";
      }
    }

    return theme('item_list', array('items' => $items));
  }
}
