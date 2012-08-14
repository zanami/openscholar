<?php 

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
  node_save($node); 
  
  if (($vsite = spaces_load('og', $node->nid)) && ($preset = spaces_preset_load($preset))) {
    spaces_preset_save_from_space($preset, $vsite);
  }
 
  return $vsite;
}

/**
 * Gets all vsites owned by uid or by current user
 **/
function vsite_get_vsite_by_owner($uid = NULL) {
  if (!$uid) {
    global $user;
    $uid = $user->uid;
  }
  
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'node')
    ->propertyCondition('uid', (int)$uid)
    ->entityCondition('bundle', 'vsite')  
    ->execute();
    
  $vsites = array();  
  if (isset($result['node'])) {
    foreach(array_keys($result['node']) as $id) {
      $vsites[] = spaces_load('og', $id);
    }
  }
      
  return array_filter($vsites);
}
