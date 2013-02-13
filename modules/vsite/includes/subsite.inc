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

  return ($vsite && in_array($space->group->type,$subsite_parents) && spaces_access_admin() && user_access("{$op} subsites"));
}