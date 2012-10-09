<?php
/**
 * @file
 *
 * Provides the Class for Feeds Token Fetcher.
 */

/**
 * Base class for the token fetcher.
 */
class FeedsTokenizedFetcher extends FeedsFetcher {

  public function fetch(FeedsSource $source) {
    $feed_node = node_load($source->feed_nid);
    $account = user_load($feed_node->uid);
    $url = token_replace($this->config['tokenized_url'], array('node' => $feed_node));
    $result = drupal_http_request($url);
    if ($result->code == 200) {
      return new FeedsFetcherResult($result->data);
    }
    else {
      // oops... error!
      drupal_set_message(t('Unable to get feeds from : ' . $url));
      return FALSE;
    }
  }

  public function configDefaults() {
    return array('tokenized_url' => '');
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = array();
    $form['tokenized_url'] = array(
      '#type' => 'textarea',
      '#title' => t('Tokenized URL'),
      '#description' => t('The URL that data will be fetched from. All tokens will be replaced before the HTTP call is made.'),
      '#default_value' => $this->config['tokenized_url'],
    );
    $form['tokenized_help'] = array(
      '#type' => 'markup',
      '#value' => theme('token_help', 'user'),
    );
    return $form;
  }
}
