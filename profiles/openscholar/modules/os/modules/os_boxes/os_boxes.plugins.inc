<?php

function os_boxes_boxes_plugins() {
  $info = array();
  $path = drupal_get_path('module', 'os_boxes').'/plugins/';

  $info['os_boxes_default'] = array(
    'handler' => array(
      'class' => 'os_boxes_default',
      'file' => 'os_boxes_default.inc',
      'path' => $path,
      'parent' => 'box',
    ),
  );

  $info['os_boxes_addthis'] = array(
    'title' => 'AddThis',
    'description' => 'Social networking links',
    'tags' => array('Social'),
    'handler' => array(
      'class' => 'os_boxes_addthis',
      'file' => 'os_boxes_addthis.inc',
      'path' => $path.'/os_boxes_addthis',
      'parent' => 'os_boxes_default',
    ),
  );
  
  $info['os_boxes_manual_list'] = array(
    'title' => 'Featured Posts',
    'description' => 'Create your own list of posts',
    'tags' => array('Content'),
    'handler' => array(
      'class' => 'os_boxes_manual_list',
      'file' => 'os_boxes_manual_list.inc',
      'path' => $path.'/os_boxes_manual_list',
      'parent' => 'os_boxes_default',
    ),
  );

 /* $info['os_boxes_media'] = array(
    'title' => 'Embed Media',
    'description' => 'Embed a video, audio or other internet resource',
    'tags' => array('Media'),
    'handler' => array(
      'class' => 'os_boxes_media',
      'file' => 'os_boxes_media.inc',
      'path' => $path.'/os_boxes_media',
      'parent' => 'os_boxes_default',
    ),
  );*/

  $info['os_boxes_booktoc'] = array(
    'title' => 'Booklet: Table of Contents',
    'description' => 'The table of contents for a booklet',
    'tags' => array('Content'),
    'handler' => array(
      'class' => 'os_boxes_booktoc',
      'file' => 'os_boxes_booktoc.inc',
      'path' => $path.'/os_boxes_booktoc',
      'parent' => 'os_boxes_default',
    ),
  );
  
  $info['os_boxes_twitterfeed'] = array(
    'title' => 'Twitter',
    'description' => 'Shows recent tweets',
    'tags' => array('Social'),
	  'module dependencies' => array('twitter_pull'),
    'handler' => array(
      'class' => 'os_boxes_twitterfeed',
      'file' => 'os_boxes_twitterfeed.inc',
      'path' => $path.'/os_boxes_twitterfeed',
      'parent' => 'os_boxes_default',
    ),
  );

  return $info;
}