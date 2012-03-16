<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function scholar_publications_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "boxes" && $api == "box") {
    return array("version" => 1);
  }
  elseif ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_imagecache_default_presets().
 */
function scholar_publications_imagecache_default_presets() {
  $items = array(
    'book_cover' => array(
      'presetname' => 'book_cover',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '150',
            'height' => '100%',
            'upscale' => 0,
          ),
        ),
      ),
    ),
    'book_cover_thumb' => array(
      'presetname' => 'book_cover_thumb',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '40',
            'height' => '100%',
            'upscale' => 0,
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function scholar_publications_views_api() {
  return array(
    'api' => '2',
  );
}

/**
 * Implementation of hook_wysiwyg_fields_icons().
 */
function scholar_publications_wysiwyg_fields_icons() {
  return array(
    drupal_get_path('module', 'scholar_publications') . '/images/wysiwyg_fields',
  );
}

/**
 * Implementation of os_simpleview_list_plugin
 * Provides publication form for simpleview list
 */
function scholar_publications_os_simpleview_list_plugin() {
	$info = array(
  	'biblio' => array( 
	  'content_type' => 'biblio',
  		'handler' => array(
  			'class' => 'scholar_publication',
  			'file'  => 'scholar_publication.inc',
  			'path'  => drupal_get_path('module', 'scholar_publications') . '/plugins/',
	      'parent' => 'default', //parent plugin, NOT parent class
  		)
  	)
  );
	
	return $info;
}