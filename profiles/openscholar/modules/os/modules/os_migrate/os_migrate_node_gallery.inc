<?php 

class GalleryNodeOSMigration extends AbstractNodeOSMigration {
  public function __construct() {
    $this->cck_fields = array();
    
    // Let AbstractNode handle the source, dest, and map.
    parent::__construct('media_gallery', 'gallery'); 
    //in d7, images are fields of the media gallery.  in 6, images were a separate content type.  errr.
    
    
    //a separate import for projects?
    $this->description = t('OS galleries');    
    
    //$this->addSimpleMappings(array('purl'));
    
    //gropu_group is this a group?
    //group_access public?
    
    //media_gallery has some features os_image_gallery lacked.  lets set some defaults here since there's nothing to import.
    $defaults = array(
      'media_gallery_columns' => 4,
      'media_gallery_rows' => 4,
      'media_gallery_allow_download' => TRUE,
      'media_gallery_expose_block' => 0, //boxes will handle this for us
      'media_gallery_block_columns' => 1,
      'media_gallery_block_rows' => 1,
    );
    foreach ($defaults as $field => $value) {
      $this->addFieldMapping($field)->defaultValue($value);
    } 
    
    //copy d6 files
    $associated_files_args = MigrateFileFieldHandler::arguments(NULL, 'file_copy', FILE_EXISTS_REPLACE);
    $this->addFieldMapping('media_gallery_media', 'prepared_files')->arguments($associated_files_args);
    
  }
  
  public function sourceFields() {
    $source_fields =  array(
      'prepared_files' => t('Images in gallery, built during prepare_row'),
    );
    
    return $source_fields;
  }
  
  public function query() {
    $query = parent::d6_db_select('node', 'n')->fields('n', $this->getNodeFields());
    $query->condition('n.type', 'gallery', '=');
    

    
    //$query->join('purl', 'p', 'n.nid = p.id');
    //$query->fields('p', array('value', 'provider'));
    //node abstract includes revisions table.  not an option for vsites.  define query here to avoid the abstract one.
    return $query;
    
    
  }
  function prepareRow($current_row) {
    $current_row;
    
    //In testing there were a handful of instances of images in someone else's gallery.
    //Views' spaces_current_space prevented those from showing up in the gallery.
    //Let's just remove them here instead by checking against the og group id. 
    $og_id = parent::d6_db_select('og_ancestry')
      ->fields('og_ancestry', array('group_nid'))
      ->condition('nid', $current_row->nid, '=')
      ->execute()
      ->fetchField();
      
    //grab associated images.  
    $query = parent::d6_db_select('node', 'n');
        
    $query->join('content_type_image', 'cti', 'n.nid = cti.field_gallery_nid');
    $query->condition('field_gallery_nid', $current_row->nid, '=');
    $query->addField('cti', 'field_gallery_image_list', 'list');
    $query->addField('cti', 'field_gallery_image_data', 'description');
    
    $query->join('files', 'f', 'f.fid = cti.field_gallery_image_fid');
    $query->fields('f', array('filepath', 'timestamp'));
    
    $query->join('node', 'img', 'img.nid = cti.nid');
    $query->addField('img', 'title');
    
    //gallery will be sorted in order that these images are added.  therefore we order them by d6's view sort parameters
    $query->orderBy('img.sticky', 'DESC'); 
    $query->orderBy('img.created', 'ASC');
    $query->orderBy('img.title', 'ASC');
    
    //see $og_id above
    $query->join('og_ancestry', 'img_og', 'img.nid = img_og.nid');
    $query->condition('img_og.group_nid', $og_id, '=');
    
    $result = $query->execute();
    $current_row->prepared_files = parent::os_migrate_prepare_files($result);

    $current_row->purl = array(
    	'value' => $current_row->value,
      'provider' => $current_row->provider,
      'id' => $current_row->nid,
    );
    
  } 
}