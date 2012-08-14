<?php 

class VsiteNodeOSMigration extends AbstractNodeOSMigration {
  public function __construct() {
    
    // Let AbstractNode handle the source, dest, and map.
    parent::__construct('vsite', 'vsite'); //current types are vsite, article, and page.  lets start with one of those instead?
    //a separate import for projects?
    $this->description = t('OS vsites');    
    $this->dependencies = preg_grep('/^VsiteNodeOS$/', $this->dependencies, PREG_GREP_INVERT);    
    
    //$this->addSimpleMappings(array('purl'));
    
    $this->addFieldMapping('field_site_description', 'og_description');
    $this->addFieldMapping('field_site_description:language', 'og_language')->defaultValue('und');
    $this->addFieldMapping('field_site_description:format')->defaultValue('plain_text');

    $this->addFieldMapping('field_site_address', 'field_vsite_address_value');
    $this->addFieldMapping('field_site_address:language', 'og_language')->defaultValue('und');
    $this->addFieldMapping('field_site_address:format', 'field_vsite_address_format')->defaultValue('plain_text');
    
    $this->addFieldMapping('group_access', 'og_private')->defaultValue(0);
    $this->addFieldMapping('group_group')->defaultValue(1);
    
    //$this->addFieldMapping('field_site_logo', 'field_site_logo');
    $this->addUnmigratedDestinations(array('field_site_logo'), t('TODO'));
    
    //undo what nodeabstract has done
    $unmap = array_merge(array('body', 'group_nid', 'og_group_ref',),  
        parent::subfields('body', array('format', 'language', 'summary')));
    foreach ($unmap as $destination_field) {   
      $this->removeFieldMapping($destination_field);
    }
    
    $this->addUnmigratedSources(array('value', 'provider', 'format'), t('DNM'));
    $this->addUnmigratedSources(parent::subfields('field_vsite_logo', array('fid','list','data'), '_'));
  }
  
  public function query() {
    $query = parent::d6_db_select('node', 'n')->fields('n');
    $query->condition('n.type', 'vsite', '=');

    $query->join('content_type_vsite', 'ct', 'n.nid = ct.nid');
    $query->fields('ct', parent::subfields('field_vsite_address', array('value', 'format'), '_'));
    
    $query->join('purl', 'p', 'n.nid = p.id');
    $query->fields('p', array('value', 'provider'));
    $query->condition('p.provider', 'spaces_og', '='); //domain sites had an extra entry for vsite_domain.  add those elsewehre later

//     $query->join('og_ancestry', 'oga', 'n.nid = oga.nid'); //node membership
//     $query->fields('oga', array('group_nid'));
    
    $query->join('og', 'og', 'n.nid = og.nid');
    $query->fields('og', parent::subfields('og', array('description', 'language', 'private'), '_'));    
    
    return $query;
  }
   function prepareRow($current_row) {
     return;    
   } 
  
   function prepare($entity, $current_row) {
     //purl isn't a valid destination yet, so just attach this to the entity.
     $entity->purl = array(
         'value' => $current_row->value,
         'provider' => $current_row->provider,
         'id' => $current_row->nid,
     );
   }

}