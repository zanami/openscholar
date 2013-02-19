<?php

/**
 * Returns if a user has permission to add/delete a subsite
 *
 * @param $op
 * Operation (create, delete)
 *
 * @param $vsite
 *
 * @return bool
 * Access
 */
function vsite_subsite_access($op = 'create', $vsite = false){

  if(!$vsite){
    $vsite = spaces_get_space();
  }

  $subsite_parents = variable_get('subsite_parents',array('department'));

  return ($vsite && in_array($vsite->group->type,$subsite_parents) && spaces_access_admin() && user_access("{$op} subsites"));
}

/**
 * Returns the subsites for a site
 *
 * @param $vsite
 * Parent Site (Vsite Object or Int)
 *
 * @param $reset
 *
 * @return array
 * Subsite ID's
 */
function vsite_get_subsites($vsite = false, $reset = false){
  $subsite_map = drupal_static('vsite_get_subsites',array());

  if($reset || (!$subsite_map && !($subsite_map = cache_get('vsite_subsite_map')))){
    $subsite_map = array();

    if($info = field_info_field('field_group_parent')){
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

  if(is_object($subsite_map)){
    $subsite_map = $subsite_map->data;
  }

  if(!$vsite && !($vsite = spaces_get_space())){
    return array();
  }

  $sid = is_object($vsite)? $vsite->group->nid:$vsite;

  return isset($subsite_map[$sid])? $subsite_map[$sid]:array();
}

/**
 * Returns if the current site has subsites
 *
 * @param $vsite
 * Parent Site (Vsite Object or Int)
 *
 *
 * @return boolean
 *
 */
function vsite_has_subsites($vsite = false){

  //If this site has subsites return True.
  return count(vsite_get_subsites($vsite))? TRUE:FALSE;
}