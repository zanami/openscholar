<?php

/**
 * @file
 * 
 * Abstract migration class for nodes
 **/


/**
 * Abstract node migration class
 * 
 * Defines mappings for node table.  Let each type implement this and dscribe the specifics
 */
abstract class NodeAbstractOSMigration extends OSMigration {
  public function __construct($bundle, $create = TRUE) {
    
    parent::__construct();    
    

    // if desired, supply node with defaults.  
    if ($create) {   
      $this->source = $this->source($bundle); //condtion type?
      $this->destination = $this->destination($bundle); 
      $this->map = $this->map($bundle);
      $this->dependencies = array('UsersOS'); //og when that's done
      
      /* mappings */
      $this->addSimpleMappings(array('nid', 'uid','created','changed','status','promote','sticky','language','comment',
                                     'title', 'tnid'));
      $this->addFieldMapping('is_new')->defaultValue(TRUE);
      $this->addFieldMapping('revision_uid', 'nr_uid');
      $this->addFieldMapping('body', 'body'); //from nr
      $this->addFieldMapping('log', 'log'); //from nr
      $this->addFieldMapping('revision', 'vid');
      $this->addFieldMapping('path', 'path'); //see prepareRow
      
      $associated_files_args = MigrateFileFieldHandler::arguments(NULL, 'file_copy', FILE_EXISTS_REPLACE);
      $this->addFieldMapping('field_file_upload', 'file_upload')->arguments($associated_files_args);
      
      /* Unmapped source fields.  Mark them as Do Not Map, so the UI knows they're intentionally ignored */
      $this->addUnmigratedSources(array('moderate', 'translate', 'type'), t('DNM'));
      //moderate appears to have been removed from node.
      //type.  we're doing nodes one type at a time.  type should be handled there.
      //translate unknown
      
    }
    //else - user is responsible for doing parent:query() and setting up source, destination, and map
  }
  
  /**
   * several helper functions for setting up child nodes.  
   * it's not clear to me yet what level of abstraction I'll be working at
   * so for now I'm leaivng it up to the node that implements this class 
   * to use the provided options or come up with their own.
   */
  
  public function query($bundle) {
    $query = parent::d6_db_select('node', 'n')
                    ->fields('n', $this->getNodeFields())
                    ->condition('n.type', $bundle, '=');
    
    $query->join('node_revisions', 'nr', 'n.vid = nr.vid');
    $query->addField('nr','body');
    $query->addField('nr','uid');    
    $query->addField('nr','log');
    $query->orderBy('n.changed');
               
    return $query;
  }
  
  public function sourceFields() {
    return array(
    	'nid'   => t('Node ID'),
      'path'  => t('URL Path Alias'),
      'file_upload' => t('File attachments'),
      //'nr.body'	=> t('Node\'s Content')
    );
  }
  
  public function source($bundle) {
    return new MigrateSourceSQL($this->query($bundle), $this->sourceFields(), null, array('map_joinable' => FALSE));
  }
  
  public function destination($bundle) {
    return new MigrateDestinationNode($bundle);
  }
  
  public function map($bundle) {
    return new MigrateSQLMap($this->machineName,
      array(
      	'nid' => array(
          'type' => 'int',
          'not null' => TRUE,
          'unsigned' => TRUE,
          'description' => 'D6 ' . $bundle . ' ID.',
          'alias' => 'n',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );
  }
  /**
   * @function getNodeFields()
   *
   *	helper function returns array of fields in node
   **/  
  function getNodeFields() {
    return array('nid', 'vid', 'type','language','title','uid','status','created',
                 'changed','comment','promote','moderate','sticky','tnid','translate',
    );
  }
  



  /**
   * Implements prepareRow
   * 
   * Fetches path from {url_alias}
   */
  function prepareRow($current_row) {
    
    //get path from {url_alias}
    $query = parent::d6_db_select('url_alias', 'ua')
      ->fields('ua', array('dst'))
      ->condition('ua.src', 'node/'.$current_row->nid, '=');
    
    $results = $query->execute();
    foreach($results as $row) {      //should be only one...
      $current_row->path = $row->dst;
    }
    
    
    //get file uploads
    $current_row->file_upload = parent::os_migrate_get_file_uploads($current_row->vid);
    // file upload fields aren't in current page type.  
  }
}
