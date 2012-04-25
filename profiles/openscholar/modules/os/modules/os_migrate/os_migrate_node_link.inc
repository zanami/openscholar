<?php 

class LinkNodeOSMigration extends AbstractNodeOSMigration {
  public function __construct() {
    //name of cck_field -> its cck field type 
    $this->cck_fields = array(
      array(
        'cck_name' => 'link_href', //src is field_link_href_url
        'cck_type' => 'Link',
        'dest_name' => 'links', //dest is field_links_link
        'dest_type' => 'Link',
      ),
    );
  
    $this->bundle = 'link'; //destination node type    
    
    // Let AbstractNode handle the source, dest, and map.
    parent::__construct($this->bundle, 'link'); //current types are vsite, article, and page.  lets start with one of those instead?
    $this->description = t('OS Link Nodes');    
    
    parent::addMappingsCCK();
  }


  //also include content type's data
  public function query($content_type) {
    $query = parent::query($content_type);
    
    parent::addQueryCCK($query, $content_type);

    return $query;
  }
  
  public function prepareRow($row) {
    parent::prepareRowCCK($row);
  }
  
  public function sourceFields() {
    $fields = parent::sourceFields();
    $fields += parent::sourceFieldsCCK();
    return $fields;
  }
}