<?php
// $Id$

/**
 * @file
 * Page callbacks and helper functions for gkplus's website.
 */

/**
 * Menu callback; displays all people nodes by term
 */
function gkplus_people_page() {
  $view_id = 'os_profiles';
  $display = 'page';

  // Sets context to 'people_people', from the custom os_people feature module.
  $context = context_load('profiles_profiles');
  context_set('context', 'profiles_profiles', $context);

  // Displays the default people view if we aren't on gkplus's site.
  if (!_gkplus_check_purl()) {
    return views_page($view_id, $display);
  }

  $name = "Research Group";
  $vocab = _gkplus_get_vocab_by_name($name);
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

  // Displays the People view across jQuery UI tabs; one tab per term.
  return _gkplus_tagged_node_tabs($term_tree);
}

/**
 * Returns a view display split by taxonomy term across jQuery UI tabs.
 *
 * jQuery UI Tabs expect this kind of DOM:
 *
 * @begincode
 * <div id="tabs">
 * 	<ul>
 * 		<li><a href="#tabs-1">Tab 1</a></li>
 * 		<li><a href="#tabs-2">Tab 2</a></li>
 * 	</ul>
 * 	<div id="tabs-1">
 * 		<!-- Content -->
 *   </div>
 * 	<div id="tabs-2">
 * 		<!-- More Content -->
 * 	</div>
 * </div>
 * @endcode
 *
 * @see gkplus_people_page().
 */
function _gkplus_tagged_node_tabs($term_tree, $bundles = array('person'), $view_mode = 'teaser') {
  $output = '<div id="people-tabs"><ul>';

  // First, processes tabs link list
  foreach ($term_tree as $term) {
    if (!$term->name) {
      continue;
    }
    $output .= '<li><a href="#gkplus-term-' . $term->tid . '">';
    $output .= $term->name;
    $output .= '</a></li>';
  }
  $output .= '</ul>';

  // Last, processes tabs pane div contents
  foreach ($term_tree as $term) {
    if (!$term->name) {
      continue;
    }
    $variables = array(
      'taxonomy_term' => $term,
      'options' => array(
        'view_mode' => $view_mode,
        'bundles' => $bundles,
      ),
    );
    $output .= '<div id="gkplus-term-' . $term->tid . '" class="taxonomy-term-child">';
    $output .= theme('taxonomy_term_tagged_node', $variables);
    $output .= '</div>';
  }
  $output .= '</div>';
  return $output;
}

/**
 * Returns the vocab object in this space with the given name, or FALSE if none.
 *
 * @see gkplus_people_page()
 */
function _gkplus_get_vocab_by_name($name) {
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