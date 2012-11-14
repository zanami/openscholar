<?php
/**
 * @file
 * os_faq.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function os_faq_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: primary-menu:faq
  $menu_links['primary-menu:faq'] = array(
    'menu_name' => 'primary-menu',
    'link_path' => 'faq',
    'router_path' => 'faq',
    'link_title' => 'Frequently asked questions',
    'options' => array(
      'attributes' => array(
        'title' => 'Frequently asked questions',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Frequently asked questions');


  return $menu_links;
}
