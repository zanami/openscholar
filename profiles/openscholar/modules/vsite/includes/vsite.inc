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
 * 	Type of object to create.  
 */
function vsite_create_vsite($name, $domain, $uid) {
  $node = new stdClass();
  $node->type = 'vsite'; 
  node_object_prepare($node);
  
  $node->title = $name;
  $node->body = array('und' => array(array('value' => '')));
  $node->purl = array('value' => $domain, 'provider'=>'vsite_og', 'id'=>NULL);
  $node->uid = $uid;
  node_save($node); 

  return $node;
}

/**
 * Gets all vsites owned by uid
 **/
function vsite_get_vsite_by_owner($uid) {
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'node')
    ->propertyCondition('uid', (int)$uid)
    ->entityCondition('bundle', 'vsite') //@TODO 
    ->execute();
    
  $vsites = array();  
  if (isset($result['node'])) {
    foreach(array_keys($result['node']) as $id) {
      $vsites[] = spaces_load('og', $id);
    }
  }
      
  return array_filter($vsites);
}
