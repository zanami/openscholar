<?php

/**
 * AJAX Page callback; gets or sets the parsed notifications feed items.
 *
 * Stores the feed items in the cache table for the specified duration
 * if called via jQuery POST with the correct 'items' data.
 *
 * Otherwise,
 */
function os_notifications_data() {
  $data = array();
  // First tries to set the cache from passed POST data.
  if (isset($_POST['items'])) {
    $items = $_POST['items'];
    $items = drupal_json_decode($items);
    if (!empty($items)) {
      // Before saving, validate that the data is trusted.
      $secret = variable_get('os_notifications_secret', '');
      $new_secret = user_password();
      variable_set('os_notifications_secret', $new_secret);
      $trusted = (bool)($_POST['os_notifications_secret'] == $secret);
      if ($trusted) {
        $expire = time() + OS_NOTIFICATIONS_CACHE_EXPIRE;
        cache_set('os_notifications_items', $items, 'cache', $expire);
      }
    }
  }

  if ($cache = cache_get('os_notifications_items')) {
    if (isset($cache->data)) {
      $items = $cache->data;
    }
  }

  if (!isset($items)) {
    $items = array();
  }

  $data['items'] = $items;
  $data['max'] = variable_get('os_platform_notifications_max', 0);
  $data['url'] = variable_get('os_platform_notifications_feeds', '');
  if (isset($_GET['os_notifications']) && $_GET['os_notifications'] == 'view') {
    drupal_json_output($data);
    die;
  }

  return $data;
}
