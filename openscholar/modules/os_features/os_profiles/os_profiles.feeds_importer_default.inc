<?php
/**
 * @file
 * os_profiles.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function os_profiles_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'os_people';
  $feeds_importer->config = array(
    'name' => 'OS people',
    'description' => 'Importing people from node.',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => FALSE,
        'directory' => 'public://feeds',
        'allowed_schemes' => array(
          0 => 'public',
        ),
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'OsImporterNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => 0,
        'authorize' => 1,
        'mappings' => array(
          0 => array(
            'source' => 'First name',
            'target' => 'field_first_name',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'Middle name',
            'target' => 'field_middle_name_or_initial',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'Last name',
            'target' => 'field_last_name',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'Titles',
            'target' => 'field_professional_title',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'Address',
            'target' => 'field_address',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'Phone',
            'target' => 'field_phone',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'Email',
            'target' => 'field_email',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'Websites title	',
            'target' => 'field_website:title',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'Websites url',
            'target' => 'field_website:url',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'Short bio',
            'target' => 'body',
            'unique' => FALSE,
          ),
          10 => array(
            'source' => 'Files',
            'target' => 'file',
            'unique' => FALSE,
          ),
          11 => array(
            'source' => 'Terms',
            'target' => 'og_vocab',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'file_field' => 'field_upload',
        'bundle' => 'person',
      ),
    ),
    'content_type' => 'person',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['os_people'] = $feeds_importer;

  return $export;
}