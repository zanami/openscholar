<?php 

/**
 * Add the passed entity to a vsite
 *
 *  The entity that you wish to add to the vsite
 *  @param entity $entity
 *  
 *  The type of entity you are passing (default node)
 *  @param string $entity_type
 *  
 *  The vsite object to add the entity to
 *  @param space $vsite
 */
function vsite_add_entity($entity, $entity_type = 'node', $vsite = false){

  //Make sure the entity is valid
  list($id) = entity_extract_ids($entity_type, $entity);

  //Get the active space
  if(!$vsite){
    $vsite = spaces_get_space();
  }
  
  if($id && $vsite){

    $values = array(
        'entity_type' => $entity_type,
        'entity' => $entity,
    );

    //Add this entity to the active group
    return og_group($vsite->group_type, $vsite->id, $values);
  }
  
  return false;
}

/**
 * @function vsite_create_vsite
 * 
 * Creates a new vsite 
 * 
 * 
 * @param string $name
 * 	Name of site
 * @param string $domain
 * 	Path for new site
 * @param string $uid
 * 	Owner
 * @param string $preset
 * 	Type of vsite to create.
 * 
 * @return vsite $vsite
 */
function vsite_create_vsite($name, $domain, $uid, $preset = FALSE) {
  $node = new stdClass();
  $node->type = 'vsite'; 
  node_object_prepare($node);
  
  $node->title = $name;
  $node->body = array('und' => array(array('value' => '')));
  $node->purl = array('value' => $domain, 'provider'=>'vsite_og', 'id'=>NULL);
  $node->uid = $uid;
  $node->spaces_preset_og = $preset; 
  node_save($node); 
  
  if ($vsite = spaces_load('og', $node->nid)) {
    return $vsite;
  }
}

/**
 * Gets all vsites a user is a member of
 * 
 * The UID of the user to lookup
 * Defaults to current user
 * @param $uid
 **/
function vsite_get_vsite_by_owner($uid = NULL) {

  //Retrieve all group memberships for the user
  $groups = og_get_entity_groups('user',$uid);
    
  $vsites = array();  
  if (isset($groups['node'])) {
    foreach($groups['node'] as $mid => $gid) {
      //Load the space object
      $vsites[]  = spaces_load('og', $gid);
    }
  }
      
  //Filter out any spaces that were not loaded
  return array_filter($vsites);
}
