<?php

class OgUserOsMigration extends OSMigration {

  public function __construct() {
    parent::__construct(MigrateGroup::getInstance('og'));

    $this->description = t('Create user relation to group.');
    $this->dependencies[] = 'VsiteNodeOSMigration';
    $this->dependencies[] = 'UsersOSMigration';

    $this->query = $query = parent::d6_db_select('og_uid', 'ogu')
      ->fields('ogu');

    $source_key = array(
      'nid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'node ID of group.',
      ),
      'uid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'User ID of member.',
      ),
    );

    $this->map = new MigrateSQLMap($this->machineName, $source_key, MigrateDestinationOGMembership::getKeySchema());
    $this->source = new MigrateSourceSQL($this->query);
    $this->destination = new MigrateDestinationOGMembership();

    $field_names = array(
      'is_admin',
      'created',
    );
    foreach ($field_names as $field_name) {
      $this->addFieldMapping($field_name, $field_name);
    }

    $this->addFieldMapping('state', 'is_active');

    $this->addFieldMapping('group_type', NULL)->defaultValue('node');
    $this->addFieldMapping('gid', 'nid');

    $this->addFieldMapping('entity_type', NULL)->defaultValue('user');
    $this->addFieldMapping('etid', 'uid');
  }

  public function prepareRow($row) {
    $row->state = !empty($row->state) ? OG_STATE_ACTIVE : OG_STATE_PENDING;
    parent::prepareRow($row);
  }
}