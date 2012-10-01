<?php
// $Id$

/**
 * @file
 * Page callbacks and helper functions for Gary's website.
 */

/**
 * Implements hook_init().
 */
function gking_init() {
  jquery_ui_add(array('ui.tabs'));
}

/**
 * Menu callback; displays all people nodes by term
 */
function gking_people_page() {
  $view_id = 'os_profiles';
  $display = 'page';

  // Sets context to 'people_people', from the custom os_people feature module.
  $context = context_load('profiles_profiles');
  context_set('context', 'profiles_profiles', $context);

  // Displays the default people view if we aren't on Gary's site.
  if (!_gking_is_gking()) {
    return views_page($view_id, $display);
  }

  $name = "Research Group";
  $vocab = _gking_get_vocab_by_name($name);
  if (!$vocab) {
    return views_page($view_id, $display);
  }

  // Setting $parent '0' loads the entire vocabulary.
  $parent        = 0;
  // Setting $max_depth NULL loads all levels.
  $max_depth     = NULL;
  $load_entities = TRUE;
  $term_tree = taxonomy_get_tree($vocab->vid, $parent, $max_depth, $load_entities);
  if (count($term_tree) === 0) {
    return views_page($view_id, $display);
  }
  $view_id = 'gking_profiles';
  // Displays the People view across jQuery UI tabs; one tab per term.
  return _gking_filter_view_into_tabs($view_id, $display, $term_tree);
}

/**
 * Returns a view display split by taxonomy term across jQuery UI tabs.
 *
 * @see gking_people_page().
 */
function _gking_filter_view_into_tabs($view_id, $display, $term_tree) {
  $output = '';
  $processed_terms = array();

  foreach ($term_tree as $term) {
    if (!$term->name) {
      continue;
    }

    // @todo use theme('taxonomy_term_tagged_node', $variables);
    $args = array($term->tid);
    $view = views_get_view($view_id);
    $view->set_display($display);

    $output .= '<div id="gking-term-' . $term->tid . '" class="taxonomy-term-child">';
    $output .= $view->execute_display($display, $args);
    $output .= '</div>';

    array_push($processed_terms, array('name' => $term->name, 'tid' => $term->tid));
  }
  // Gives javascript file access to the term IDs to create tabs with jQuery UI.
  drupal_add_js(array('gking' => array('research_group_terms' => $processed_terms)), 'setting');
  dpm($output);
  return $output;
}

/**
 * Returns the vocab object in this space with the given name, or FALSE if none.
 *
 * @see gking_people_page()
 */
function _gking_get_vocab_by_name($name) {
  // Includes API function os_get_vocabularies().
  $include = drupal_get_path('module', 'os') . '/includes/taxonomy.inc';
  include_once ($include);

  $vocabs = os_get_vocabularies();

  foreach ($vocabs as $vid => $vocab) {
    if ($vocab->name === 'Research Group') {
      return $vocab;
    }
  }
  return FALSE;
}