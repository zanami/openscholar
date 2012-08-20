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
    if (preg_match('/_flavor$/', $row->object_id)) {
      $this->_update_flavors($row);
    }
  }
  
  /*
   * @function _update_flavors
   * 
   * Updates flavor variables and names.
   */
  public function _update_flavors(&$row) {
    $row->object_id = preg_replace('/vsite_design_scholar/', 'os_appearance', $row->object_id);
    
    $flavor = unserialize($row->value);
    $flavor = preg_replace('/^scholar_/', '', $flavor);
    $row->value = serialize($flavor);
  }
}

