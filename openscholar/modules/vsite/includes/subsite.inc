<?php

/**
 * Returns if a user has permission to add/delete a subsite.
 *
 * @param $op
 *   Operation (create, delete)
 * @param $vsite
 *   Optional; the Vsite node object.
 *
 * @return
 *   TRUE if user has access.
 */
function vsite_subsite_access($op = 'create', $vsite = FALSE){
  if (!$vsite = vsite_get_vsite($vsite)) {
    return;
  }

  if (!spaces_access_admin() || !user_access("{$op} subsites")) {
    return;
  }

  $subsite_parents = variable_get('subsite_parents',array('department'));

  return in_array($vsite->group->type, $subsite_parents);
}

/**
 * Returns the subsites for a site.
 *
 * @param $vsite
 *   Parent Site (Node object or integer).
 *
 * @return
 *   Array with subsite IDs.
 */
function vsite_get_subsites($vsite = FALSE) {
  $subsite_map = drupal_static(__FUNCTION__ ,array());

  if(!$subsite_map && !$subsite_map = cache_get('vsite_subsite_map')) {
    $subsite_map = array();

    if (field_info_field('field_group_parent')){
      $result = db_select('field_data_field_group_parent', 'fgp')
        ->fields('fgp',array('bundle','entity_id','field_group_parent_target_id'))
        ->condition('deleted', 0)
        ->execute();

      while ($item = $result->fetch()){
        $subsite_map[$item->field_group_parent_target_id][$item->entity_id] = $item->entity_id;
      }
    }

    cache_set('vsite_subsite_map',$subsite_map);
  }

  if (is_object($subsite_map)){
    $subsite_map = $subsite_map->data;
  }

  if (!$vsite = vsite_get_vsite($vsite)) {
    return array();
  }

  $sid = $vsite->group->nid:$vsite;

  return isset($subsite_map[$sid])? $subsite_map[$sid]: array();
}

/**
 * Returns the url options that should be used for a cross-site link to a node
 *
 * ie. A link to a subsite
 *
 * @param $vsite
 * Active Site (Vsite Object)
 *
 *
 * @return boolean
 *
 */
function vsite_subsite_node_url_options($node, &$options){
  if (!$vsite = vsite_get_vsite()) {
    return;
  }

  if (!isset($node->{OG_AUDIENCE_FIELD})){
    // No group information.
    return;
  }

  //Check and see if this node is part of a subsite "not this site"
  $groups = field_get_items('node', $node, OG_AUDIENCE_FIELD);
  if (isset($groups[0]) && $vsite->id != $groups[0]['target_id']){
    //Absolute will force the use of "viste_domain_name"
    $options['absolute'] = TRUE;
    //This will rewrite the URL based on this nodes group
    $options['purl'] = array('id' => $groups[0], 'provider' => 'spaces_og');
  }
}