<?php
/**
 * @file
 * os_reader.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function os_reader_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'os_reader';
  $feeds_importer->config = array(
    'name' => 'OS reader',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:0' => 'image',
          'xpathparser:1' => 'title',
          'xpathparser:2' => 'description',
          'xpathparser:3' => 'pubDate',
        ),
        'rawXML' => array(
          'xpathparser:0' => 0,
          'xpathparser:1' => 0,
          'xpathparser:2' => 0,
          'xpathparser:3' => 0,
        ),
        'context' => '//item',
        'exp' => array(
          'errors' => 0,
          'tidy' => 0,
          'tidy_encoding' => 'UTF8',
          'debug' => array(
            'context' => 'context',
            'xpathparser:0' => 'xpathparser:0',
            'xpathparser:1' => 'xpathparser:1',
            'xpathparser:2' => 0,
            'xpathparser:3' => 0,
          ),
        ),
        'allow_override' => 1,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'OsFeedReaderFetcher',
      'config' => array(
        'type' => array(),
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:0',
            'target' => 'image',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'xpathparser:1',
            'target' => 'title',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'xpathparser:2',
            'target' => 'description',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'xpathparser:3',
            'target' => 'time',
            'unique' => FALSE,
          ),
        ),
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => 1800,
    'expire_period' => 3600,
    'import_on_create' => TRUE,
    'process_in_background' => FALSE,
  );
  $export['os_reader'] = $feeds_importer;

  return $export;
}
