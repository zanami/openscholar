<?php 

class AnnouncementNodeOSMigration extends AbstractNodeOSMigration {
  public function __construct() {
    
    $this->cck_fields = array(
      array(
        'cck_name' => 'announcement_photo', 
        'cck_type' => 'File',
        'dest_name' => 'field_photo', 
        'dest_type' => 'Image',
      ),
    );
    
    // Let AbstractNode handle the source, dest, and map.
    parent::__construct('announcement', 'announcement'); //current types are vsite, article, and page.  lets start with one of those instead?
    $this->description = t('OS Announcement Nodes');    

  }
  

}