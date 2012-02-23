<?php 

class NodePageOSMigration extends NodeAbstractOSMigration {
  public function __construct() {
    
    // Let AbstractNode handle the source, dest, and map.
    parent::__construct('page'); //current types are vsite, article, and page.  lets start wit hone of those instead?
    $this->description = t('OS Page Nodes');    

    /************************************************************************
     *  Field mappings tell Migrate which d7 column to deposit d6 data into *
     ************************************************************************/
    
  }

}