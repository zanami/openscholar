<?php

/**
 * @file
 * Implements TableCopyMigration for each of the biblio tables we're copying.
 */

class BiblioContributorTypeTableMigration extends TableCopyMigration {
  public function __construct() {
    $key_schema = array(
        'auth_category' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
        ),
        'biblio_type' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
        ),
        'auth_type' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
        ),
    );

    $table = 'biblio_contributor_type';

    parent::__construct($table, $key_schema);
  }
}


class BiblioContributorTypeDataTableMigration extends TableCopyMigration {
  public function __construct() {

    $key_schema = array(
        'auth_type' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
        ),
    );

    $table = 'biblio_contributor_type_data';

    parent::__construct($table, $key_schema);
  }
}

class BiblioTypesTableMigration extends TableCopyMigration {
  public function __construct() {
    $key_schema = array(
      'tid' => array(
        'type' => 'int',
        'not null' => TRUE,
      ),
    );

    $table = 'biblio_types';

    parent::__construct($table, $key_schema);
  }
}

class BiblioFieldTypeTableMigration extends TableCopyMigration {
  public function __construct() {
    $key_schema = array(
      'tid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'unsigned' => TRUE,
      ),
      'fid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'unsigned' => TRUE,
      ),
    );

    $table = 'biblio_field_type';

    parent::__construct($table, $key_schema);
  }
}

class BiblioFieldTypeDataTableMigration extends TableCopyMigration {
  public function __construct() {
    $key_schema = array(
      'ftdid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'unsigned' => TRUE,
      ),
    );

    $table = 'biblio_field_type_data';

    parent::__construct($table, $key_schema);
  }
}

