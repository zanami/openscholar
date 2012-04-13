<?php 

//copy inline files into file entities
class FileInlineOSMigration extends OSMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Inline Files');
    $this->dependencies = array();
    $this->source = new MigrateSourceSQL($this->query(), array(), NULL,  array('map_joinable' => FALSE));
    $this->map = new MigrateSQLMap($this->machineName,
      array('field_os_inline_files_fid' => array('type'=>'int', 'unsigned'=>TRUE, 'not null'=>TRUE)),
      MigrateDestinationFile::getKeySchema());
      
    $this->destination = new MigrateDestinationFile(array('copy_file'=>TRUE));
    
    // Field mappings //
    
    $this->addSimpleMappings(array('uid', 'filename', 'filemime', 'status', 'timestamp'));
    //file entity's uri and path are unclear
    //assuming uri is full url (with http) to reach the file.  relative path shouldn't work unless there's some voodoo i haven't found handling those
    //path alias.  assuming its the file alias name rather than its actual name
    $this->addFieldMapping('uri', 'filepath');
    $this->addFieldMapping('path', 'filename');
    
    //ignored
    $this->addUnmigratedDestinations(array('contents'), t('DNM')); //only used in copying blobs to files
    
    //data from inilne table.  may yet need it during prepare or joins
    $this->addUnmigratedSources(array('vid', 'nid', 'delta', 'field_os_inline_files_fid', 'field_os_inline_files_list', 'field_os_inline_files_data'), t('DNM'));
    
  }
  
    
  public function query() {
    $query = parent::d6_db_select('content_field_os_inline_files', 'cf')
      ->fields('cf', array('vid', 'nid', 'delta', 'field_os_inline_files_fid', 'field_os_inline_files_list', 'field_os_inline_files_data'))
      ->isNotNull('field_os_inline_files_fid');
    $query->join('files', 'f', 'cf.field_os_inline_files_fid = f.fid');
    $query->fields('f', array('uid', 'filename', 'filepath', 'filemime', 'status', 'timestamp'));
      
    //path, uri
    return $query;
  }
  
/*  public function prepareRow($row) {
    
  }*/
  

}

//for migrating media, MigrateDesinationMedia extends File...
