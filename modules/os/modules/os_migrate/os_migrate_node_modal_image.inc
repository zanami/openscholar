<?php 

/**
 * @file os_migrate_node_stub.inc
 */

class ModalImageNodeOSMigration extends OSMigration {
  public function __construct() {
    
    parent::__construct(); // nope!  AbstractNode assumes node->node
    $this->dependencies = array('UsersOS');
    
    $this->destination = new MigrateDestinationMedia();
    $this->description = t('Modal image node -> media migration class.');
    $this->source = new MigrateSourceSQL($this->query(), $this->sourceFields(), null, array('map_joinable' => FALSE));
    $this->map = new MigrateSQLMap($this->machineName, 
      array( 'nid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'unsigned' => TRUE,
        'description' => 'D6 modal_image node ID.',
        'alias' => 'n',
        )
    ),
      MigrateDestinationMedia::getKeySchema()
    );

    $this->addSimpleMappings(array('uid', 'timestamp', 'path'));
    $this->addUnmigratedDestinations(array('destination_dir', 'destination_file', 'file_replace', 'preserve_files', 'source_dir'), t('DNM')); //good defaults
    
    $this->addFieldMapping('value', 'filepath');
    $this->addFieldMapping('source_dir')->defaultValue(variable_get('os_migrate_files', FALSE)); //adequate or does 2.4 need full path?
    $this->addFieldMapping('og_group_ref', 'group_nid');
    
  }
  
  public function query() {
    $query = parent::d6_db_select('node', 'n')
      ->fields('n', array('nid', 'uid'));
    $query->condition('n.type', 'modal_image');
    
    $query->join('content_type_modal_image', 'ct', 'n.vid = ct.vid');
    $query->fields('ct');
    
    $query->join('files', 'f', 'ct.field_modal_image_fid = f.fid');
    $query->fields('f');
    
    $query->join('og_ancestry', 'og', 'n.nid = og.nid');
    $query->fields('og', array('group_nid'));
    $query->condition('og.group_nid', '0', '!=');

    /**
     * Modify the query.  Add joins and fields here.  content_type_${content_type} and its fields are already included.
     */
    
    return $query;
  }
  
  public function sourceFields() {
    return array(
      'uid' => t('Node owner\'s uid'),
      'filename' => t('Name of file'),
      'filepath' => t('Path to file'),
      'timestamp' => t('File creation timestamp'),
      'path' => t('URL alias'),
      );
  }
  
  public function prepareRow($current_row) {
    //get path from {url_alias}
    $query = parent::d6_db_select('url_alias', 'ua')
    ->fields('ua', array('dst'))
    ->condition('ua.src', 'node/'.$current_row->nid, '=');
    $results = $query->execute()->fetchAssoc();
    $current_row->path = $results['dst'];

    
  }
}