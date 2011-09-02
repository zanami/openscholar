<?php

/**
 * recursive function to build the tree of terms
 * @see vsite_taxonomy_vocabs
 */
function theme_vsite_taxonomy_term_list(&$index, $tree, $s_term_theme = 'vsite_taxonomy_term', $b_description = true, $b_show_empty = true, $b_show_admin_links = true, $s_term_feature = false, $b_feature_context = false){
  $items = array();

  $a_filter_nodes = array();
  if($s_term_feature){
    $node_map = features_get_component_map('node');
    foreach ($node_map as $s_node => $a_features) {
      if(in_array($s_term_feature,$a_features)) $a_filter_nodes[] = $s_node;
    }
  }//Retrieve nodes for a feature if needed

  $current_depth = $tree[$index]->depth;
  while ( $index < count($tree) && $tree[$index]->depth >= $current_depth ) {
    $term = $tree[$index];
    $count = vsite_taxonomy_term_count_nodes($term->tid,false,$a_filter_nodes);

    if ($tree[$index + 1]->depth > $current_depth) {
      $index ++;
      if($b_show_empty || $count || theme_vsite_taxonomy_term_list($term->tid,true,$a_filter_nodes)) {
        $items[] = array(
          'data' => theme($s_term_theme, $term, $b_description, $b_show_admin_links, $s_term_feature, $b_feature_context),
          'children' => vsite_taxonomy_build_list_items($index, $tree, $s_term_theme, $b_description, $b_show_empty, $b_show_admin_links, $s_term_feature, $b_feature_context)
        );
      }
    }
    else {
      if($b_show_empty || $count) $items[] = theme($s_term_theme, $term, $b_description, $b_show_admin_links, $s_term_feature, $b_feature_context);
      $index ++;
    }
  }
  return $items;
}