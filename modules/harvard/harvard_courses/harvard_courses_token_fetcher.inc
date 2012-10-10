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
    $wrapper = entity_metadata_wrapper('node', $source->feed_nid);
    $nids = haravard_courses_sites_nid_by_import_type($wrapper->field_import_type->value());

    if (empty($nids)) {
      drupal_set_message(t('There were no sites for this import.'));
      return FALSE;
    }

    // Define the query paramater and the filed that holds the query values.
    if ($wrapper->field_import_type->value() == 'cat_num') {
      $field = 'field_catalog_number';
      $url_param = 'cat_num';
    }
    else {
      $field = 'field_department_id';
      $url_param = 'field';
    }

    // Loop over the nodes and build the query paramter value.
    $param_values = array();
    foreach ($nids as $nid) {
      $import_wrapper = entity_metadata_wrapper('node', $nid);
      foreach ($import_wrapper->{$field}->value() as $import_values) {
        $param_values[] = is_array($import_values) ? $import_values['value'] : $import_values;
      }
    }

    // Build the url for import and verify for response code 200 from the url.
    $url = $wrapper->field_base_url->value() . '?' . $url_param . '=' . implode($param_values, ',');
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
}
