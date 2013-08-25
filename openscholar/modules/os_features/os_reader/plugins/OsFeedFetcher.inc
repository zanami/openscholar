<?php
/**
 * @file
 *
 * Provides the Class for Feeds Token Fetcher.
 */
/**
 * Base class for the token fetcher.
 */
class OsFeedFetcher extends FeedsFetcher {

  public function fetch(FeedsSource $source) {
    $wrapper = entity_metadata_wrapper('node', $source->feed_nid);

    if (empty($wrapper->field_rss_url)) {
      return new FeedsFetcherResult('<xml></xml>');
    }

    // Build the URL for import and verify for response code 200 from the URL.
    $url = $this->config['debug_mode'] ? variable_get('purl_base_domain') . '/' . drupal_get_path('module', 'os_reader') . '/includes/dummy_rss.xml' : $wrapper->field_rss_url->value();
    $result = drupal_http_request($url);

    if ($result->code != 200) {
      // oops... error!
      drupal_set_message(t('Unable to get feeds from: @url', array('@url' => $wrapper->field_rss_url->value())));
      return new FeedsFetcherResult('<xml></xml>');
    }

    return new FeedsFetcherResult($result->data);
  }

  public function configDefaults() {
    return array('debug_mode' => '');
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = array();
    $form['debug_mode'] = array(
      '#type' => 'checkbox',
      '#title' => t('Debug mode'),
      '#description' => t('When this is checked, the importer will import courses from a dummy file.'),
      '#default_value' => isset($this->config['debug_mode']) ? $this->config['debug_mode'] : FALSE,
    );
    return $form;
  }
}
