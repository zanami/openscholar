<?php

// /**
//  * @file
//  *
//  * Migration class
//  **/

/**
 * Vocab table migration class
 */
class VocabularyOSMigration extends TableCopyMigration {
  public function __construct() {

    $this->description = t('OS taxonomy vocab migration');

    $key_schema = array(
        'vid' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
        ),
    );

    $table = 'vocabulary';
    $dest_table = 'taxonomy_vocabulary';
    $group = MigrateGroup::getInstance('taxonomy');

    parent::__construct($table, $key_schema, $group, $dest_table);

  }

  /**
   * Implements prepareRow
   *
   **/
  public function prepareRow($current_row) {

    //New Machine name field
    $current_row->machine_name = "vocabulary_".$current_row->vid;
    Migration::registerMigration('TermOSMigration', 'TermOSMigration'.$current_row->machine_name, array('dst_vocabulary' => $current_row->machine_name, 'src_vid' => $current_row->vid));
    //No longer used
    unset($current_row->relations);
    unset($current_row->help);
    unset($current_row->multiple);
    unset($current_row->required);
    unset($current_row->tags);

    return TRUE;  // FALSE to skip a row.
  }

  /**
   * Implements complete
   *
   **/
  public function complete($vocabulary) {

  }

}


