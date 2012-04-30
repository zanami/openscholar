<?php 

class AnnouncementNodeOSMigration extends AbstractNodeOSMigration {
  public function __construct() {
    
    $this->bundle = 'announcement';
    $this->cck_fields = array(
      array(
        'cck_name' => 'announcement_photo', 
        'cck_type' => 'File',
        'dest_name' => 'field_photo', 
        'dest_type' => 'Image',
      ),
    );
    
    // Let AbstractNode handle the source, dest, and map.
    parent::__construct($this->bundle, 'announcement'); //current types are vsite, article, and page.  lets start with one of those instead?
    $this->description = t('OS Announcement Nodes');    

    /************************************************************************
     *  Field mappings tell Migrate which d7 column to deposit d6 data into *
     ************************************************************************/
    parent::addMappingsCCK();
    
  }
  
  //also include content type's data
  public function query($content_type) {
    $query = parent::query($content_type);
    
    parent::addQueryCCK($query, $content_type);
        
//    $query->leftJoin('content_type_announcement', 'cta', 'n.vid = cta.vid');
//    $query->fields('cta', array('field_announcement_photo_fid', 'field_announcement_photo_list', 'field_announcement_photo_data'));
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