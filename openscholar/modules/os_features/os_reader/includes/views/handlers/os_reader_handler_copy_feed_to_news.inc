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
      if (os_reader_is_feed_item_copied_to_vsite($values->id, $vsite->nid)) {
        $items[] = t('Feed item already copied to @vsite', array('@vsite' => $vsite->title));
      }
      else {
        $token = drupal_get_token($values->id . ':' . $vsite->nid);
        $options = array(
          'attributes' => array(
            'class' => array('use-ajax'),
          ),
        );
        $copy = t('Copy to @vsite', array('@vsite' => $vsite->title));

        $url = l($copy, 'os-reader/copy-feed-to-news/' . $values->id . '/' . $vsite->nid . '/nojs/' . $token, $options);
        $items[] = "<span id='feed-{$values->id}-{$vsite->nid}'>$url</span>";
      }
    }

    return theme('item_list', array('items' => $items));
  }
}
