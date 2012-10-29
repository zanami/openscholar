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

  public function complete($entity) {
    $this->nid = $entity->nid;
    $this->featureUpdate(array_keys(spaces_features()), 1);
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
 * Migrating books.
 */
class OsMigratePublicationNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'biblio';

  public $csvColumns = array(
    array('id', 'ID'),
    array('title', 'Title'),
    array('biblio_place_published', 'Publisher'),
    array('biblio_year', 'Year'),
    array('biblio_place_published', 'Publish location'),
    array('biblio_type_name', 'Biblio type name'),
    array('biblio_type', 'Biblio type'),
    array(OG_AUDIENCE_FIELD, 'Site'),
  );

  public $dependencies = array(
    'OsMigratePersonalNode',
  );

  public function __construct() {
    parent::__construct();

    $this->addFieldMapping('Publisher', 'biblio_place_published');
    $this->addFieldMapping('Year', 'biblio_year');
    $this->addFieldMapping('Publish location', 'biblio_place_published');
    $this->addFieldMapping('Biblio type name', 'biblio_type_name');
    $this->addFieldMapping('Biblio type', 'biblio_type');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration('OsMigratePersonalNode');
  }

  /**
   * When creating a new publication, the biblio module mange the fields
   * diffrently and consider them as properties and not fields. The prepare
   * function will deal with that.
   */
  public function prepare($entity, $row) {
    foreach ($row as $row_key => $row_vallue) {
      if (in_array($row_key, array('id', 'title', 'og_group_ref', 'uid'))) {
        continue;
      }
      $entity->$row_key = $row_vallue;
    }
  }
}

/**
 * Migrating classes.
 */
class OsMigrateClassesNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'class';

  public $csvColumns = array(
    array('id', 'ID'),
    array('title', 'Title'),
    array('body', 'Body'),
    array('link', 'Link'),
    array('semester', 'Semester'),
    array(OG_AUDIENCE_FIELD, 'Site'),
  );

  public $dependencies = array(
    'OsMigratePersonalNode',
  );

  public function __construct() {
    parent::__construct();

    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('field_semester', 'semester');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration('OsMigratePersonalNode');
  }

  /**
   * Dealing with the link url field.
   */
  public function prepare($entity, $row) {
    $link = explode(";", $row->link);

    $entity->field_class_link[LANGUAGE_NONE][0]['url'] = $link[0];
    $entity->field_class_link[LANGUAGE_NONE][0]['title'] = $link[1];
  }
}

/**
 * Migrate classes material.
 */
class OsMigrateClassesMaterialNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'class_material';

  public $csvColumns = array(
    array('id', 'ID'),
    array('title', 'Title'),
    array('body', 'Body'),
    array('field_class', 'Class'),
    array(OG_AUDIENCE_FIELD, 'Site'),
  );

  public $dependencies = array(
    'OsMigratePersonalNode',
    'OsMigrateClassesNode',
  );

  public function __construct() {
    parent::__construct();

    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration('OsMigratePersonalNode');
    $this->addFieldMapping('field_class', 'field_class')
      ->sourceMigration('OsMigrateClassesNode');
  }
}

/**
 * Migrate curriculum vitae.
 */
class OsMigrateCurriculumVitaeNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'cv';

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

    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration('OsMigratePersonalNode');
  }
}

/**
 * Migrate news.
 */
class OsMigrateNewsNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'news';

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

    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration('OsMigratePersonalNode');
  }
}

/**
 * Migrate events.
 */
class OsMigrateEventsNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'event';

  public $csvColumns = array(
    array('id', 'ID'),
    array('title', 'Title'),
    array('body', 'Body'),
    array('timestamp', 'Timestamp'),
    array(OG_AUDIENCE_FIELD, 'Site'),
  );

  public $dependencies = array(
    'OsMigratePersonalNode',
  );

  public function __construct() {
    parent::__construct();

    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('field_date', 'timestamp');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration('OsMigratePersonalNode');
  }

  public function prepare($entity, $row) {
    // Handling the date field with the exported timestamps in the csv.
    $timesatamps = explode(";", $row->timestamp);
  }
}

/**
 * Migrate Blog.
 */
class OsMigrateBlogNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'blog';

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

    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration('OsMigratePersonalNode');
  }
}

/**
 * Migrate documents.
 */
class OsMigrateBookNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'book';

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

    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration('OsMigratePersonalNode');
  }
}

/**
 * Migrate people.
 */
class OsMigratePeopleNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'person';

  public $csvColumns = array(
    array('id', 'ID'),
    array('title', 'First name'),
    array('middle_name', 'Middle name'),
    array('last_name', 'Last name'),
    array('phone', 'Phone'),
    array('professional_titles', 'Professional titles'),
    array('address', 'Address'),
    array('email', 'E-mail'),
    array('websites', 'Websites'),
    array('body', 'Body'),
    array(OG_AUDIENCE_FIELD, 'Site'),
  );

  public $dependencies = array(
    'OsMigratePersonalNode',
  );

  public function __construct() {
    parent::__construct();

    $this->addFieldMapping('field_middle_name_or_initial', 'middle_name');
    $this->addFieldMapping('field_last_name', 'last_name');
    $this->addFieldMapping('field_phone', 'phone');
    $this->addFieldMapping('field_email', 'email');
    $this->addFieldMapping('field_address', 'address');
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('field_professional_title', 'professional_titles')->separator(';');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration('OsMigratePersonalNode');
  }

  public function prepare($entity, $row) {
    // Insertint the first name - title - into the first name field.
    $entity->field_first_name[LANGUAGE_NONE][0]['value'] = $entity->title;

    // Handeling websites.
    if (!empty($row->websites)) {
      $websites = explode(';', $row->websites);
      foreach ($websites as $key => $website) {
        $link = explode('|', $website);

        $entity->field_website[LANGUAGE_NONE][$key] = array(
          'title' => $link[0],
          'url' => $link[1],
        );
      }
    }
  }
}

// TODO: OG vocab migration class have problem. Need to fix it.
/**
 * Migrate OG-vocabs.
 */
// class OsMigrateOgVocab extends OsMigrate {
//
  // /**
   // * Map the field and properties to the CSV header.
   // */
  // public $csvColumns = array(
    // array('id', 'ID'),
    // array('name', 'Name'),
  // );
//
  // public $entityType = 'og_vocab';
  // public $bundle = 'og_vocab';
//
  // public $dependencies = array(
    // 'OsMigratePersonalNode',
    // 'OsMigrateBiologyTaxonomyTerm',
    // 'OsMigrateMathTaxonomyTerm',
    // 'OsMigrateScienceTaxonomyTerm',
  // );
//
// }
