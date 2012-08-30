<?php 

class AnnouncementNodeOSMigration extends AbstractNodeOSMigration {
  public function __construct() {

    
    // Let AbstractNode handle the source, dest, and map.
    parent::__construct('announcement', 'announcement'); //current types are vsite, article, and page.  lets start with one of those instead?
    $this->description = t('OS Announcement Nodes');    
    
    $this->addSimpleMappings(array('field_photo', 'field_photo:language', 'field_photo:alt', 'field_photo:title', 'field_photo:source_dir'));
    
    $this->addUnmigratedDestinations(array('field_photo:file_class', 'field_photo:destination_dir', 'field_photo:destination_file', 'field_photo:file_replace',
        'field_photo:preserve_files', ), t('DNM'));

  }
  
  public function query($content_type) {
    $query = parent::query($content_type);
  
    /**
     * Modify the query.  Add joins and fields here.
    */

  
    return $query;
  }
  
  public function prepareRow($row) {
    if ($row->field_announcement_photo_data && $data = unserialize($row->field_announcement_photo_data)) {
      $row->{'field_photo:alt'} = isset($data['alt']) ? $data['alt'] : '';
      $row->{'field_photo:title'} = isset($data['title']) ? $data['title'] : '';
    }
    
    $lang = 'field_photo:language';
    $row->$lang = $row->language;
    
    //get attached photo
    $result = parent::d6_db_select('files', 'f')
    	->condition('fid', $row->field_announcement_photo_fid, '=')
    	->fields('f')
    	->execute();
    
    $photo = $result->fetchAssoc();
    if (is_array($photo) && count($photo)) {
      $row->field_photo = $photo->filename;
      $row->{'field_photo:source_dir'} = $photo->filepath;
    } else {
      $row->field_photo = $row->{'field_photo:source_dir'} = NULL;
    }
    
    return parent::prepareRow($row); //false -> skip this row
  }

}