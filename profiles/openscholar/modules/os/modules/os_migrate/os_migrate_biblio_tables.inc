<?php

/**
 * @file 
 * Implements TableCopyMigration for each of the biblio tables we're copying.
 */

class BiblioContributorTableMigration extends TableCopyMigration {
  public function __construct() {
    
    $key_schema = array(
      'vid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
    	),
      'cid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'auth_type' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'rank' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),        
    );
    
    $table = 'biblio_contributor';
    
    parent::__construct($table, $key_schema);
  }
}