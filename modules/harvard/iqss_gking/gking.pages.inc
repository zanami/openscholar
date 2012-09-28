<?php
// $Id$

/**
 * @file
 * Page callbacks and helper functions for Gary's website.
 */

/**
 * Menu callback; displays all people nodes by term
 */
function gking_people_page() {
  $view = 'os_people';
  $display = 'page';

  // Sets context to 'people_people', from the custom os_people feature module.
  $context = context_load('profiles_profiles');
  context_set('context', 'profiles_profiles', $context);

  // Displays the default people view if we aren't on Gary's site.
  if (!_gking_is_gking()) {
    return views_page($view, $display);
  }

  $name = "Research Group";
  $vocab = _gking_get_vocab_by_name($name);
  if (!$vocab) {
    return views_page($view, $display);
  }

  $term_tree = taxonomy_get_tree($vocab->vid, 0, -1, 1);
  if (count($term_tree) === 0) {
    return views_page($view, $display);
  }

  // Displays the People view across jQuery UI tabs; one tab per term.
  return _gking_filter_view_into_tabs($view, $display, $term_tree);
}

/**
 * Returns a view display split by taxonomy term across jQuery UI tabs.
 *
 * @see gking_people_page().
 */
function _gking_filter_view_into_tabs($view, $display, $term_tree) {
  $output = '';
  $processed_terms = array();
  $views_tid_filter = array(
    'operator' => 'and',
    'value' => array(),
    'group' => 0,
    'exposed' => FALSE,
    'limit' => TRUE,
    'vid' => $vocab->vid,
    'id' => 'tid',
    'table' => 'term_node',
    'field' => 'tid',
    'hierarchy' => 0,
    'relationship' => 'none',
    'reduce_duplicates' => 0,
  );

  foreach ($term_tree as $term) {
    if (!$term->name) {
      continue;
    }

    $view = views_get_view($view);
    $view->set_display($display);
    $views_tid_filter['value'] = array(0 => $term->tid);
    $view->display_handler->default_display->options['filters']['tid'] = $views_tid_filter;
    $view->display_handler->definition['use pager'] = FALSE;
    $view->display_handler->options['defaults']['items_per_page'] = FALSE;

    $output .= '<div id="gking_term_' . $term->tid . '" class="taxonomy-term-child">' . $view->execute_display($display) . "</div>";
    array_push($processed_terms, array('name' => $term->name, 'tid' => $term->tid));
    $view->destroy();
  }
  // Gives javascript file access to the term IDs to create tabs with jQuery UI.
  drupal_add_js(array('gking' => array('research_group_terms' => $processed_terms)), 'setting');

  return $output;
}

/**
 * Returns the vocab object in this space with the given name.
 *
 * @see gking_people_page()
 */
function _gking_get_vocab_by_name($name) {
  $vsite  = vsite_get_vsite();
  $vocabs = $vsite->get_vocabs('person');

  foreach ($vocabs as $vocab) {
    if ($vocab->name === 'Research Group') {
      return $vocab;
    }
  }
  return FALSE;
}