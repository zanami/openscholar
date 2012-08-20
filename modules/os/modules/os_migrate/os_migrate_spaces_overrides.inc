<?php

/**
 * @file 
 * Implements TableCopyMigration for each of the biblio tables we're copying.
 */

class SpacesOverridesTableMigration extends TableCopyMigration {
  public function __construct() {
    $key_schema = array(
        'type' => array(
            'type' => 'varchar',
            'length' => 64,
            'not null' => TRUE,
        ),
        'id' => array(
            'type' => 'varchar',
            'length' => 255,
            'not null' => TRUE,
        ),
        'object_type' => array(
            'type' => 'varchar',
            'length' => 64,
            'not null' => TRUE,
        ),
        'object_id' => array(
            'type' => 'varchar',
            'length' => 255,
            'not null' => TRUE,
        ),
    );

    $table = 'spaces_overrides';

    parent::__construct($table, $key_schema);
  }
  
  public function prepareRow(&$row) {
    if ($row->object_type == 'variable') {
      switch($row->object_id) {
        case 'foo':
        break;
      }
    }
    TRUE;
  }
}

