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
      array('fid' => array('type'=>'int', 'unsigned'=>TRUE, 'not null'=>TRUE)),
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
    $fid_query = parent::d6_db_select('content_field_os_inline_files', 'cf')
      ->fields('cf', array('field_os_inline_files_fid'))
      ->isNotNull('field_os_inline_files_fid')
      ->distinct();
        
    $query = parent::d6_db_select('files', 'f')
      ->fields('f', array('uid', 'filename', 'filemime', 'status', 'timestamp', 'fid', 'filepath'))
      ->condition('f.fid', $fid_query, 'IN');
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
      ->isNotNull('field_os_inline_image_fid')
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




class EmbedInlineOSMigration extends OSMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Inline Embeds');
    $this->dependencies = array();
    $add_fields = array('uri' => 'Unique Resource Identifier.', 'filemime' => 'File\'s mime type', 'filename' => 'File name');
    $this->source = new MigrateSourceSQL($this->query(), $add_fields , NULL,  array('map_joinable' => FALSE));
    $this->map = new MigrateSQLMap($this->machineName,
      //array('migrate_md5_uri' => array('type'=>'varchar', 'length'=>255)), //varchar 2048 is base, but too big for here
      array('vid' => array('type'=>'int', 'unsigned'=>TRUE, 'not null'=>TRUE,)),
      MigrateDestinationMedia::getKeySchema());
      //file dest is no good heree.  oembed:// is unsupported
    
    $this->destination = new MigrateDestinationFile(array('copy_file'=>TRUE));
      
    //mappings
    $this->addSimpleMappings(array('uid', 'status', 'uri', 'filemime', 'filename'));
    $this->addFieldMapping('timestamp', 'created');
    
    //ignored
    $this->addUnmigratedSources(array('field_os_inline_oembed_url'), 'DNM');
    $this->addUnmigratedDestinations(array('contents'), 'DNM'); //use this if we hvae to create a file.
    
  }
  
  public function query() {
    $query = parent::d6_db_select('content_field_os_inline_oembed', 'cf')
      ->fields('cf', array('field_os_inline_oembed_url', 'vid'))
      ->isNotNull('field_os_inline_oembed_url')
      ->groupBy('field_os_inline_oembed_url');
    $query->leftJoin('node', 'n', 'cf.nid = n.nid');
    $query->fields('n', array('uid', 'status', 'created'));
    return $query;
  }
  
  public function prepareRow($row) {
    $row->uri = preg_replace('/https?:\/\//', 'oembed://', $row->field_os_inline_oembed_url);//$row->field_os_inline_oembed_url;
    $row->filename = basename($row->uri); //or fetch this from media's name getting funcs.  slow as hell?
    
  }
  
  //destination forces public:// URI.  switch it back.  uri is also truncated to basename.  lets not make all youtube links the same, kthxbai
  public function prepare($entity, $row) { 
    $entity->uri = $row->uri;//preg_replace('/https?:\/\//', 'oembed://', $row->field_os_inline_oembed_url);
    
  }
}