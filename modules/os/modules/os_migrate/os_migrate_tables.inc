<?php

/**
 * @file
 * Implements TableCopyMigration for several direct table copies
 */

class IQSSPinserverRegisterTableMigration extends TableCopyMigration {

  public function __construct() {
    $this->required_modules = array('iqss_pinserver_register');
    $key_schema = array(
        'uid' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
        ),
    );

    $table = 'iqss_pinserver_register';
    parent::__construct($table, $key_schema);
  }
}


class CitationDistributeTableMigration extends TableCopyMigration {
  public function __construct() {
    $this->required_modules = array('citation_distribute');

    $key_schema = array(
      'nid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'plugin' => array(
        'type' => 'varchar',
        'length' => 64,
        'not null' => TRUE,
      ),
    );

    $table = 'citation_distribute';

    parent::__construct($table, $key_schema);
  }
}

class VariableTableMigration extends TableCopyMigration {

  public function __construct() {
    $this->whitelist = array('mollom_private_key', 'mollom_public_key', 'os_mailchimp_api', 'os_mailchimp_list', 'olark_code');

    $this->whitelist += parent::subfields('comment', parent::d6_list_content_types(), '_');
    $this->whitelist += parent::subfields('citation_distribute', array('dash_password', 'module_mode', 'service_googlescholar_auto_flag', 'service_repec_archive_code', 'service_repec_inst_name', 'service_repec_maintainer_email', 'service_repec_maintainer_name', 'service_repec_provider_homepage', 'service_repec_provider_institution', 'service_repec_title'), '_');

    // the following look like they should be migrated, but I'm not sure
    //pinserver*, shorten*,

    $key_schema = array(
      'name' => array(
        'type' => 'varchar',
        'length' => 128,
        'not null' => TRUE,
      ),
    );

    $table = 'variable';
    parent::__construct($table, $key_schema);
  }

  public function query() {
    $query = parent::query();
    $query->condition('name', $this->whitelist, 'IN');
    return $query;
  }
}
