<?php

/**
 * Migrate "Biology" terms.
 */
class OsMigrateBiologyTaxonomyTerm extends OsMigrate {

  /**
   * Map the field and properties to the CSV header.
   */
  public $csvColumns = array(
    array('id', 'ID'),
    array('name', 'Name'),
  );

  public $entityType = 'taxonomy_term';
  public $bundle = 'biology';
}

/**
 * Migrate "Math" terms.
 */
class OsMigrateMathTaxonomyTerm extends OsMigrate {

  /**
   * Map the field and properties to the CSV header.
   */
  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('name', 'Name'),
  );

  public $entityType = 'taxonomy_term';
  public $bundle = 'math';
}

/**
 * Migrate "Science" terms.
 */
class OsMigrateScienceTaxonomyTerm extends OsMigrate {

  /**
   * Map the field and properties to the CSV header.
   */
  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('name', 'Name'),
  );

  public $entityType = 'taxonomy_term';
  public $bundle = 'science';
}

/**
 * Migrate "Personal" node.
 */
class OsMigratePersonalNode extends OsMigrate {
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
 * Migrate "Biography" node.
 */
class OsMigrateBioagrphyNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'bio';

  public $csvColumns = array(
    array('id', 'ID'),
    array('title', 'Title'),
    array('body', 'Body'),
    array(OG_AUDIENCE_FIELD, 'Site'),
  );

  public $dependencies = array(
    'OsMigratePersonalNode',
  );

  public function __construct() {
    parent::__construct();

    $this->addFieldMapping('Body', 'body');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration('OsMigratePersonalNode');
  }
}


/**
 * Migrate OG-vocabs.
 */
class OsMigrateOgVocab extends OsMigrate {

  /**
   * Map the field and properties to the CSV header.
   */
  public $csvColumns = array(
    array('id', 'ID'),
    array('name', 'Name'),
  );

  public $entityType = 'og_vocab';
  public $bundle = 'og_vocab';

  public $dependencies = array(
    'OsMigratePersonalNode',
    'OsMigrateBiologyTaxonomyTerm',
    'OsMigrateMathTaxonomyTerm',
    'OsMigrateScienceTaxonomyTerm',
  );

}
