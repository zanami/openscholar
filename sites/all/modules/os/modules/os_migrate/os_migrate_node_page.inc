<?php 

class PageNodeOSMigration extends AbstractNodeOSMigration {
  public function __construct() {
    
    // Let AbstractNode handle the source, dest, and map.
    parent::__construct('page'); //current types are vsite, article, and page.  lets start with one of those instead?
    $this->description = t('OS Page Nodes');    

    /************************************************************************
     *  Field mappings tell Migrate which d7 column to deposit d6 data into *
     ************************************************************************/
    
  }
  
  //also include content type's data
  public function query($bundle) {
    $query = parent::query($bundle);
    $query->condition('n.type', 'page', '=');
    return $query;
  }

}