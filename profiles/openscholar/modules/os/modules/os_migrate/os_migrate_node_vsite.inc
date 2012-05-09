<?php 

class VsiteNodeOSMigration extends AbstractNodeOSMigration {
  public function __construct() {
    $this->cck_fields = array();
    
    // Let AbstractNode handle the source, dest, and map.
    parent::__construct('vsite', 'vsite'); //current types are vsite, article, and page.  lets start with one of those instead?
    //a separate import for projects?
    $this->description = t('OS vsites');    
    
    $this->addSimpleMappings(array('purl'));
    
    //gropu_group is this a group?
    //group_access public?
  }
  
  public function query() {
    $query = parent::d6_db_select('node', 'n')->fields('n', $this->getNodeFields());
    $query->condition('n.type', 'vsite', '=');
    
    $query->join('purl', 'p', 'n.nid = p.id');
    $query->fields('p', array('value', 'provider'));
    //node abstract includes revisions table.  not an option for vsites.  define query here to avoid the abstract one.
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
}