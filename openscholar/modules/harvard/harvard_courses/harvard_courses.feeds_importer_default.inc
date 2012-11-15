<?php
/**
 * @file
 * harvard_courses.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function harvard_courses_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'course';
  $feeds_importer->config = array(
    'name' => 'Course',
    'description' => 'Import courses',
    'fetcher' => array(
      'plugin_key' => 'HarwardFetcher',
      'config' => array(),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:14' => 'cat_num',
          'xpathparser:0' => 'cat_num',
          'xpathparser:1' => 'description',
          'xpathparser:2' => 'faculty',
          'xpathparser:3' => 'field',
          'xpathparser:4' => 'meetings',
          'xpathparser:5' => 'notes',
          'xpathparser:6' => 'number',
          'xpathparser:7' => 'prerequisites',
          'xpathparser:8' => 'room',
          'xpathparser:9' => 'term',
          'xpathparser:11' => 'building',
          'xpathparser:12' => 'bracketed',
          'xpathparser:13' => 'title',
        ),
        'rawXML' => array(
          'xpathparser:14' => 0,
          'xpathparser:0' => 0,
          'xpathparser:1' => 0,
          'xpathparser:2' => 0,
          'xpathparser:3' => 0,
          'xpathparser:4' => 0,
          'xpathparser:5' => 0,
          'xpathparser:6' => 0,
          'xpathparser:7' => 0,
          'xpathparser:8' => 0,
          'xpathparser:9' => 0,
          'xpathparser:11' => 0,
          'xpathparser:12' => 0,
          'xpathparser:13' => 0,
        ),
        'context' => '//courses/course',
        'exp' => array(
          'errors' => 0,
          'debug' => array(
            'context' => 0,
            'xpathparser:14' => 0,
            'xpathparser:0' => 0,
            'xpathparser:1' => 0,
            'xpathparser:2' => 0,
            'xpathparser:3' => 0,
            'xpathparser:4' => 0,
            'xpathparser:5' => 0,
            'xpathparser:6' => 0,
            'xpathparser:7' => 0,
            'xpathparser:8' => 0,
            'xpathparser:9' => 0,
            'xpathparser:11' => 0,
            'xpathparser:12' => 0,
            'xpathparser:13' => 0,
          ),
        ),
        'allow_override' => 1,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'content_type' => 'harvard_course',
        'expire' => '-1',
        'author' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:14',
            'target' => 'guid',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'xpathparser:0',
            'target' => 'field_cat_number',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'xpathparser:1',
            'target' => 'field_description',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'xpathparser:2',
            'target' => 'field_faculty',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'xpathparser:3',
            'target' => 'field_field',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'xpathparser:4',
            'target' => 'field_meetings',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'xpathparser:5',
            'target' => 'field_notes',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'xpathparser:6',
            'target' => 'field_number',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'xpathparser:7',
            'target' => 'field_prerequisites',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'xpathparser:8',
            'target' => 'field_room',
            'unique' => FALSE,
          ),
          10 => array(
            'source' => 'xpathparser:9',
            'target' => 'field_term',
            'unique' => FALSE,
          ),
          11 => array(
            'source' => 'xpathparser:11',
            'target' => 'field_building',
            'unique' => FALSE,
          ),
          12 => array(
            'source' => 'xpathparser:12',
            'target' => 'field_bracketed',
            'unique' => FALSE,
          ),
          13 => array(
            'source' => 'xpathparser:13',
            'target' => 'title',
            'unique' => 0,
          ),
        ),
        'update_existing' => '2',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
      ),
    ),
    'content_type' => 'harvard_api_importer',
    'update' => 0,
    'import_period' => '1800',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['course'] = $feeds_importer;

  return $export;
}
