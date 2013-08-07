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
    return l(t('Copy'), 'ajax/os-reader/copy-feed-to-news/' . $values->id . '/' . 1 . '/nojs', array('attributes' => array('class' => array('use-ajax'))));
  }
}
