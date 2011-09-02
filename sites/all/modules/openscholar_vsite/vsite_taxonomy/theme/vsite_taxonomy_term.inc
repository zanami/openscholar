<?php

/**
 * theme a individual term
 * @param unknown_type $term
 */
function theme_vsite_taxonomy_term($term, $b_description = true, $b_admin_links = true, $s_term_feature = false, $b_feature_context = false){

  $a_filter_nodes = array();
  if($s_term_feature){
    $node_map = features_get_component_map('node');
    foreach ($node_map as $s_node => $a_features) {
      if(in_array($s_term_feature,$a_features)) $a_filter_nodes[] = $s_node;
    }
  }//Retrieve nodes for a feature if needed

  $count = vsite_taxonomy_term_count_nodes($term->tid,false,$a_filter_nodes);
  $active_term = vsite_taxonomy_active_term();

  if($count <= 0 || $active_term->tid == $term->tid){
    $term_link = $term->name;

  }else{
    $term_link = l($term->name, _vsite_taxonomy_get_term_path($term->tid,$s_term_feature, $b_feature_context), array('title' => t($term->description)));
  }

  $item = '<div class="term-head">' . $term_link;

  $item .= ($count > 0)?'<span class = "count" > (' . $count . ')</span>':"";

  if($b_admin_links && cp_access_cp()){
    // add term's admin links
    $item .= theme('links', vsite_get_admin_links('term', $term), $attributes = array(
      'class' => 'admin'
    ));
  }

  if ($b_description && $term -> description) {
    $item .= '<div class="description">' . substr($term -> description,0,58) . ((strlen($term->description) >53)?"&hellip;":"") . '</div>';
    }

  $item .= '</div><!--/term-head-->';

  return $item;
}