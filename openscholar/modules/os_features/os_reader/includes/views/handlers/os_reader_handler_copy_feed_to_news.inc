<?php

/**
 * @file
 * Contains the message field handler.
 */

/**
 * Views handler for displaying link that creates news node based on feed item.
 */
class os_reader_handler_copy_feed_to_news extends views_handler_field {
  function render($values) {

    global $user;

    $groups = og_get_entity_groups('user', $user);
    $vsite_ids = array_keys($groups['node']);
    $vsites = node_load_multiple($vsite_ids);

    $items = array();
    foreach ($vsites as $vsite) {
      $copy = t('Copy to @vsite', array('@vsite' => $vsite->title));
      if (os_reader_feed_orphan_from_node($values->id, $vsite->nid)) {
        $items[] = $copy;
      }
      else {
        $items[] = "<span id='feed-{$values->id}-{$vsite->nid}'>" .
          l($copy, 'ajax/os-reader/copy-feed-to-news/' . $values->id . '/' . $vsite->nid . '/nojs', array(
            'attributes' => array(
              'class' => array('use-ajax'),
            ),
          )) . "</span>";
      }
    }

    return theme('item_list', array('items' => $items));
  }
}
