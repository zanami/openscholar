<?php
/**
 * @file
 * harvard_courses.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function harvard_courses_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: primary-menu:courses
  $menu_links['primary-menu:courses'] = array(
    'menu_name' => 'primary-menu',
    'link_path' => 'courses',
    'router_path' => 'courses',
    'link_title' => 'Courses',
    'options' => array(
      'attributes' => array(
        'title' => 'Site\'s courses',
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
  t('Courses');


  return $menu_links;
}
