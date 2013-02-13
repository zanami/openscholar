<?php 

function os_wysiwyg_link_os_link_plugin() {
  media_include_browser_js();
  
  $plugins['os_link'] = array(
    'title' => 'Link',
    'vendor url' => 'http://openscholar.harvard.edu',
    'icon path' => '',
    'icon url' => '',
    'icon title' => 'Link',
    // @todo: move this to the plugin directory for the wysiwyg plugin.
    'js path' => drupal_get_path('module', 'os_wysiwyg_link') . '/js',
    'js file' => 'wysiwyg-os_link.js',
    'css file' => NULL,
    'css path' => NULL,
    'settings' => array(
      'global' => array(
        'enabledPlugins' => os_wysiwyg_link_get_media_browser_plugins(),
        'types' => os_wysiwyg_link_get_media_browser_types(),
        'id' => 'os_link_wysiwyg',
      ),
    ),
  );
  
  return $plugins;
}