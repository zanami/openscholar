<?php
/**
 * @file
 * Provides support for the Views module.
 */
class OsReaderViewsController extends EntityDefaultViewsController {

  /**
   * Override views_data().
   */
  public function views_data() {
    $data = parent::views_data();

    $data['os_feed_item']['copy_feed_to_news'] = array(
      'title' => t('Copy Feed to News'),
      'help' => t('Filter messages by message type category.'),
      'field' => array(
        'handler' => 'os_reader_handler_copy_feed_to_news',
        'real field' => 'id',
      ),
    );

    // Allow some HTML.
    $data['os_feed_item']['description']['field']['handler'] = 'views_handler_field_markup';
    $data['os_feed_item']['description']['field']['format'] = 'filtered_html';

    return $data;
  }
}
