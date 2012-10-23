<?php

/**
 * Migrating sites.
 */
class OsMigratePersonal extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'personal';

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('title', 'Title'),
    array('purl', 'URL'),
  );

  public function __construct() {
    parent::__construct();
    $this->addFieldMapping('purl', 'purl');
    $this->addFieldMapping('field_site_address', 'purl');
  }

  /**
   * Creating the PURL address for the personal site.
   */
  public function prepare($entity, $row) {
    $entity->purl = array('value' => $row->purl);

    $value = $entity->field_site_address[LANGUAGE_NONE][0]['value'];
    $entity->field_site_address[LANGUAGE_NONE][0]['value'] = variable_get('purl_base_domain') . '/' . $value;
  }
}

/**
 * Importing biography for the site owner.
 */
class OsMigrateBioagrphy extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'bio';

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('title', 'Title'),
    array('body', 'Body'),
    array(OG_AUDIENCE_FIELD, 'Site'),
  );

  public function __construct() {
    parent::__construct();
    $this->dependencies = array('OsMigratePersonal');
    $this->addFieldMapping('Body', 'body');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration('OsMigratePersonal');
  }
}
