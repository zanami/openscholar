<?php 

class VsiteNodeOSMigration extends AbstractNodeOSMigration {
  public function __construct() {
    
    // Let AbstractNode handle the source, dest, and map.
    parent::__construct('vsite', 'vsite'); //current types are vsite, article, and page.  lets start with one of those instead?
    //a separate import for projects?
    $this->description = t('OS vsites');    
    
    $this->addSimpleMappings(array('purl'));
    
    $this->addFieldMapping('field_site_description', 'og_description');
    $this->addFieldMapping('field_site_description:language', 'og_language')->defaultValue('und');
    $this->addFieldMapping('field_site_description:format')->defaultValue('plain_text');

    $this->addFieldMapping('field_site_address', 'field_vsite_address_value');
    $this->addFieldMapping('field_site_address:language', 'og_language')->defaultValue('und');
    $this->addFieldMapping('field_site_address:format', 'field_vsite_address_format')->defaultValue('plain_text');
    
    //gropu_group is this a group?
    //group_access public?
    $this->addFieldMapping('group_access', 'og_private')->defaultValue(0);
    $this->addFieldMapping('group_group')->defaultValue(1);
  }
  
  public function query() {
    $query = parent::d6_db_select('node', 'n')->fields('n');
    $query->condition('n.type', 'vsite', '=');

    $query->join('content_type_vsite', 'ct', 'n.nid = ct.nid');
    $query->fields('ct', parent::subfields('field_vsite_address', array('value', 'format'), '_'));
    
    $query->join('purl', 'p', 'n.nid = p.id');
    $query->fields('p', array('value', 'provider'));

//     $query->join('og_ancestry', 'oga', 'n.nid = oga.nid'); //node membership
//     $query->fields('oga', array('group_nid'));
    
    $query->join('og', 'og', 'n.nid = og.nid');
    $query->fields('og', parent::subfields('og', array('description', 'language', 'private'), '_'));    
    
    return $query;
  }
  function prepareRow($current_row) {
    
    //purl isn't a valid destination yet, but still works somehow.
    $current_row->purl = array(
    	'value' => $current_row->value,
      'provider' => $current_row->provider,
      'id' => $current_row->nid,
    );
  } 
  
//   function prepare($entity, $row) {
//     $entity;
//   }
}