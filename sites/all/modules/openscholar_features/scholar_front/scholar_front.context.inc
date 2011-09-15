<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function scholar_front_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'openscholar-feature-front';
  $context->description = 'Site front page';
  $context->tag = 'Feature';
  $context->conditions = array(
    'path' => array(
	    'values' => array(
	      '<front>' => '<front>',
	    ),
	  ),
	  'context' => array(
      'values' => array(
        '~openscholar-section-sitewide_front' => '~openscholar-section-sitewide_front',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-scholar_events_upcoming-block_1' => array(  // not changing
          'module' => 'views',
          'delta' => 'scholar_events_upcoming-block_1',
          'weight' => 20,
          'region' => 'right',
          'status' => '0',
          'label' => 'Upcoming events',
          'type' => 'context_ui',
        ),
        'boxes-scholar_announcements_latest' => array(
          'module' => 'boxes',
          'delta' => 'scholar_announcements_latest',
          'weight' => 21,
          'region' => 'right',
          'status' => '0',
          'label' => 'Latest Announcements',
        ),
        'views-vsite_featuredposts-block_1' => array(
		      'module' => 'views',
		      'delta' => 'vsite_featuredposts-block_1',
		      'weight' => 18,
		      'region' => 'right',
		      'label' => 'Featured posts',
		    ),
		    'boxes-scholar_biocv_bio_blurb' => array(
		      'module' => 'boxes',
		      'delta' => 'scholar_biocv_bio_blurb',
		      'weight' => 0,
		      'region' => 'content_bottom',
		      'label' => 'Bio Blurb',
		    ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('The front page context');

  $export['openscholar-feature-front'] = $context;
  return $export;
}
