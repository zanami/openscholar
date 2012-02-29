<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function scholar_presentations_ctools_plugin_api() {
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
  elseif ($module == 'os_simpleview_list' && $api == 'plugins') {
    return array('version' => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function scholar_presentations_node_info() {
  $items = array(
    'presentation' => array(
      'name' => t('Presentation'),
      'module' => 'features',
      'description' => t('A presentation event.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function scholar_presentations_views_api() {
  return array(
    'api' => '2',
  );
}

/**
 * Implementation of hook_wysiwyg_fields_icons().
 */
function scholar_presentations_wysiwyg_fields_icons() {
  return array(
    drupal_get_path('module', 'scholar_presentations') . '/images/wysiwyg_fields',
  );
}

/**
 * Implementation of os_simpleview_list_plugin
 * Provides presentation form for simpleview list
 */
function scholar_presentations_os_simpleview_list_plugin() {
	$info = array(
  	'scholar_presentations' => array(
  		'handler' => array(
  			'class' => 'scholar_presentations',
  			'file'  => 'scholar_presentations.inc',
  			'path'  => drupal_get_path('module', 'scholar_presentations') . '/plugins/',
	      'parent' => 'os_simpleview_list_default',
  		)
  	)
  );
	
	return $info;
}