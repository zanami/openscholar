<?php 

class SpacesOverridesBoxesMigration extends SpacesOverridesTableMigration {
  
  public function __construct() {
    $this->source_object = $this->dest_object = 'boxes';
    parent::__construct();
  }
  
  public function query() {
    $query = parent::query();
    $query->condition('object_id', '', '!=');
    return $query;
  }
  
  
  
  public function prepareRow(&$row) {
    if (($value = @unserialize($row->value)) === FALSE) {
      if (strlen($row->value) > 65535) {
        //single image saved some images inline (ie img src='data:image:png....).  these were truncated in db and don't function
        parent::saveMessage(t('Box too big, source data truncated.  Ignoring.  id=@id', array('@id'=>$row->id), MigrationBase::MESSAGE_NOTICE));
      } else {        
        parent::saveMessage(t('Can\'t unserialize box: id=@id, object_id=@object_id', 
          array('@id' => $row->id, '@object_id' => $row->object_id)), MigrationBase::MESSAGE_WARNING);
      }
      
      return FALSE;
    }
    
    if (($value = $this->_update_block((array)$value)) === FALSE) {
      return FALSE;
    } else {
      $value = (object)$value;
    }
    
    
//    $row->object_id = $object_id;
    $row->value = serialize($value);
    return parent::prepareRow($row);
  }
  
}
