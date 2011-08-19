<?php

/**
 * Implementation of hook_imagecache_default_presets().
 */
function os_slideshow_imagecache_default_presets() {
  $items = array(
    'os_slideshow_550' => array(
      'presetname' => 'os_slideshow_550',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '550',
            'height' => '350',
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
function os_slideshow_views_api() {
  return array(
    'api' => '2',
  );
}
