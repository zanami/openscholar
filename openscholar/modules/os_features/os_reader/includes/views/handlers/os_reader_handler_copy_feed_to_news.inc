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

    // TODO: Get the vsite of the user.
    $vsite = 1;

    if (os_reader_feed_orphan_from_node($values->id, $vsite)) {
      return t('Copy');
    }
    else {
      return "<span id='feed-{$values->id}'>". l(t('Copy'), 'ajax/os-reader/copy-feed-to-news/' . $values->id . '/' . $vsite . '/nojs', array(
        'attributes' => array(
          'class' => array('use-ajax'),
        ),
      )) . "</span>";
    }
  }
}
