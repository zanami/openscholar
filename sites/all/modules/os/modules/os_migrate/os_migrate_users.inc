<?php

/**
 * @file
 * 
 * Migration class 
 **/

//abstract class OSMigration extends Migration {
abstract class OSMigration extends Migration {
  public function __construct() {
    // Always call the parent constructor first for basic setup
    parent::__construct();

    // With migrate_ui enabled, migration pages will indicate people involved in
    // the particular migration, with their role and contact info. We default the
    // list in the shared class; it can be overridden for specific migrations.
    $this->team = array(
      new MigrateTeamMember('Jon Sagotsky', 'jsagotsky@fas.harvard.edu', t('Migration Lead')),
    );

    // Individual mappings in a migration can be linked to a ticket or issue
    // in an external tracking system. Define the URL pattern here in the shared
    // class with ':id:' representing the position of the issue number, then add
    // ->issueNumber(1234) to a mapping.
    $this->issuePattern = 'https://redmine.hmdc.harvard.edu/issues/:id:';
  }
  
  /**
   * @function d6_db_select
   * 
   * As db_select, but targets old d6 database
   */
  public function d6_db_select($target, $alias = NULL, array $options = array()) {
//    $options['target'] = 'd6_db';
  //  return db_select($target, $alias, $options);
  return Database::getConnection('default', 'd6_db')
    ->select($target, $alias, $options);
  }
  
}


/**
 * User table migration class
 */
class UsersOSMigration extends OSMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('OS user accounts');
    $source_fields = array('uid' => t('User ID'));
    
    /* Migrate the following fields from the users table */
    //$query = db_select('users', 'u')
    //$query = parent::get_d6_db()
    //$query = parent::d6_db_select('users', 'u')
    
    //target is necessary.  otherwise we read from default db getting 3 users.  true when nesting default>d6_db
    
    //$query = db_select('dataville.users', 'u', array('target'=>'d6_db'))
    $query = Database::getConnection('default', 'd6_db')
      ->select('users', 'u')
      ->fields('u', array('uid','name','pass','mail','mode','sort','threshold','theme',
      									  'signature','signature_format','created','access','login',
      									  'status','timezone','language','picture','init','data',
      									  'timezone_name'))
      ->condition('u.name', array('admin'), 'NOT IN')
      ->condition('u.uid', 1, '>')
    ;
      
    $this->source = new MigrateSourceSQL($query, $source_fields, null, array('map_joinable' => FALSE));
    $this->destination = new MigrateDestinationUser(array('md5_passwords'=>TRUE)); //arg3 enables us to copy old md5 passwords
    
    
    /* Migrate needs to know about the keys of the source table to track migration status */
    $this->map = new MigrateSQLMap($this->machineName,
      array(
      	'uid' => array(
          'type' => 'int',
          'not null' => TRUE,
          'unsigned' => TRUE,
          'description' => 'D6 User ID.',
          'alias' => 'u',
        ),
      ),
      MigrateDestinationUser::getKeySchema()
    );
    
    /* Field mappings tell Migrate which d7 column to deposit d6 data into */
    
    // addSimpleMappings handles the columns that haven't changed or been renamed
    $this->addSimpleMappings(array('uid','pass', 'name','mail', 'created', 'access', 'login', 'status', 'init', 'theme',
                                   'signature', 'signature_format', 'language',
    ));
    
    // addFieldMapping lets you map one row to another or perform additional functions like setting a default
    $this->addFieldMapping('timezone', 'timezone_name');
    $this->addFieldMapping('picture', 'picture')->defaultValue(0);
    $this->addFieldMapping('is_new')->defaultValue(TRUE); //not creating a new user, just copying one
    
    //tmp role map just to see what happens
//    $this->addFieldMapping('roles')->defaultValue(2);
  
    //data ??? str -> blob?
    
    //new rows in d7:
    //roles
    //group_audience
    //path (path alias)
      
    // Unmapped source fields.  Mark them as Do Not Map, so the UI knows they're intentionally ignored
   $this->addUnmigratedSources(array('mode','sort','threshold', 'timezone', 'data'), t('DNM'));
    //restore 'data' when we know what it does
    $this->addUnmigratedDestinations(array('group_audience', 'path', 'roles'), t('DNM')); //just ignore them for today
/*

    // Dedupe assures that value is unique. Use it when source data is non-unique.
    // Pass the Drupal table and column for determining uniqueness.
    $this->addFieldMapping('name', 'name')
         ->dedupe('users', 'name');

    $this->addFieldMapping('pass', 'password');
    // Instead of mapping a source field to a destination field, you can
    // hardcode a default value. You can also use both together - if a default
    // value is provided in addition to a source field, the default value will
    // be applied to any rows where the source field is empty or NULL.
    $this->addFieldMapping('roles')
         ->defaultValue(2);
    $this->addFieldMapping('field_migrate_example_gender', 'sex');

    // The source field has beer names separated by a pipe character ('|'). By
    // adding ->separator('|'), the migration will automatically break them out,
    // look up the node with each title, and assign the node reference to this
    // user.
    if (module_exists('node_reference')) {
      $this->addFieldMapping('field_migrate_example_favbeers', 'beers')
           ->separator('|');
    }

    // Unmapped source fields
    $this->addFieldMapping(NULL, 'nickname')
         ->issueGroup(t('DNM'));

    // Unmapped destination fields

    // This is a shortcut you can use to mark several destination fields as DNM
    // at once
    $this->addUnmigratedDestinations(array('theme', 'signature', 'access', 'login',
      'timezone', 'language', 'picture', 'is_new'));

    // Oops, we made a typo - this should have been 'init'! If you have
    // migrate_ui enabled, look at the BeerUser info page - you'll see that it
    // displays a warning "int used as destination field in mapping but not in
    // list of destination fields", and also lists "1 unmapped" under Destination,
    // where it highlights "init" as unmapped.
    $this->addFieldMapping('int')
         ->issueGroup(t('DNM'));
         */
  }
}