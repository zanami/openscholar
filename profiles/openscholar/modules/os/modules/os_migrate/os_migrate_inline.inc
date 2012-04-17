<?php 

//copy inline files into file entities

class FileInlineOSMigration extends OSMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Inline Files');
    $this->dependencies = array();
    $add_fields = array('uri' => 'Unique Resource Identifier.');
    $this->source = new MigrateSourceSQL($this->query(), $add_fields , NULL,  array('map_joinable' => FALSE));
    $this->map = new MigrateSQLMap($this->machineName,
      array('field_os_inline_files_fid' => array('type'=>'int', 'unsigned'=>TRUE, 'not null'=>TRUE)),
      MigrateDestinationFile::getKeySchema());
      
    $this->destination = new MigrateDestinationFile(array('copy_file'=>TRUE));
    
    // Field mappings //
    
    $this->addSimpleMappings(array('uid', 'filename', 'filemime', 'status', 'timestamp', 'uri'));
   
    
    //ignored
    $this->addUnmigratedDestinations(array('contents'), t('DNM')); //only used in copying blobs to files
    
    //data from inilne table.  may yet need it during prepare or joins
    $this->addUnmigratedSources(array('vid', 'nid', 'delta', 'field_os_inline_files_fid', 'field_os_inline_files_list', 'field_os_inline_files_data', 'filepath'), t('DNM'));
    
  }
  
    
  public function query() {
    $query = parent::d6_db_select('content_field_os_inline_files', 'cf')
      ->fields('cf', array('vid', 'nid', 'delta', 'field_os_inline_files_fid', 'field_os_inline_files_list', 'field_os_inline_files_data'))
      ->isNotNull('field_os_inline_files_fid')
      ->distinct();
    $query->join('files', 'f', 'cf.field_os_inline_files_fid = f.fid');
    $query->fields('f', array('uid', 'filename', 'filepath', 'filemime', 'status', 'timestamp'));
      
    //path, uri
    return $query;
  }
  
  public function prepareRow($row) {
    //file::import expects to recieve the file from a url or db blob.  local copy is not available.  
    //going with that for now.  may patch migrate if it's obnoxious.
    //this appraoch does not take into account vsite_domain_names from spaces_overrides
    $query = parent::d6_db_select('variable', 'v')
      ->fields('v', array('value'))
      ->condition('v.name', 'purl_base_domain', '=');
    $purl = unserialize($query->execute()->fetchField());
    $row->uri = $purl . '/' . $row->filepath; 
  }
  

}

//for migrating media, MigrateDesinationMedia extends File...




class ImageInlineOSMigration extends OSMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Inline Images');
    $this->dependencies = array();
    $add_fields = array('uri' => 'Unique Resource Identifier.');
    $this->source = new MigrateSourceSQL($this->query(), $add_fields , NULL,  array('map_joinable' => FALSE));
    $this->map = new MigrateSQLMap($this->machineName,
      array('fid' => array('type'=>'int', 'unsigned'=>TRUE, 'not null'=>TRUE,)),
      //array('os_inline_image_migrate_id' => array('type'=>'varchar'), 'length'=>255, 'not null'=>TRUE, 'description' => 'Unique ids for migrate'),
      MigrateDestinationFile::getKeySchema());
    $this->destination = new MigrateDestinationFile(array('copy_file'=>TRUE));
      
    
    // Field mappings //
    $this->addSimpleMappings(array('uid', 'filename', 'filemime', 'status', 'timestamp', 'uri'));
   
    //ignored
    $this->addUnmigratedDestinations(array('contents'), t('DNM')); //only used in copying blobs to files
  }
  
  public function query() {
    $fid_query = parent::d6_db_select('content_field_os_inline_image', 'cf')
      ->fields('cf', array('field_os_inline_image_fid'))
      ->distinct();
    
    $query = parent::d6_db_select('files', 'f')
      ->fields('f', array('uid', 'filename', 'filemime', 'status', 'timestamp', 'fid', 'filepath'))
      ->condition('f.fid', $fid_query, 'IN');
    
    return $query;
  }
  
  public function prepareRow($row) {
    //file::import expects to recieve the file from a url or db blob.  local copy is not available.  
    //going with that for now.  may patch migrate if it's obnoxious.
    //this appraoch does not take into account vsite_domain_names from spaces_overrides
    $query = parent::d6_db_select('variable', 'v')
      ->fields('v', array('value'))
      ->condition('v.name', 'purl_base_domain', '=');
    $purl = unserialize($query->execute()->fetchField());
    $row->uri = $purl . '/' . $row->filepath;

   // $row->field_os_inline_image_fid = $row->field_os_inline_image_fid . $row->vid . $row->delta; 
  }
}

