<?php
/**
 * @file
 * os_importer.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function os_importer_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'os_blog_import_rss';
  $feeds_importer->config = array(
    'name' => 'OS blog import - RSS',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'OsImporterFetcher',
      'config' => array(),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsSyndicationParser',
      'config' => array(),
    ),
    'processor' => array(
      'plugin_key' => 'OsFeedReaderFetcher',
      'config' => array(
        'type' => array(),
        'mappings' => array(
          0 => array(
            'source' => 'guid',
            'target' => 'guid',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'description',
            'target' => 'description',
            'unique' => FALSE,
          ),
        ),
      ),
    ),
    'content_type' => 'blog_import',
    'update' => 0,
    'import_period' => '1800',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['os_blog_import_rss'] = $feeds_importer;

  return $export;
}
