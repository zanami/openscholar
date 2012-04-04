<?php 

/**
 * @class OSMigration
 *
 * Abstract class for all OpenScholar migration classes
 * Sets some defaults
 * Provides d6_db_select helper for getting d6 db for import
 */
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
   * Sends users to admin page on a misconfigured db
   */
  public function d6_db_select($target, $alias = NULL, array $options = array()) {
    static $os_migrate_database;  
    if (!isset($os_migrate_database)) {
      $os_migrate_database =  variable_get('os_migrate_database', '');
    }
  
    try {
      return Database::getConnection('default', $os_migrate_database) 
        ->select($target, $alias, $options);
    } catch (DatabaseConnectionNotDefinedException $e) {
      flood_register_event('os_migrate_bad_db');
      $msg = t('Unable to connect to db %db.  Please !link', array(
      				  '%db' => $os_migrate_database, 
      				  '!link'=>l('check settings','admin/config/os_migrate')));
      drupal_set_message($msg, 'error');
                  
      throw $e;
    }
  }
    
  /**
   * Gets files owned by node revision
   */
  function os_migrate_get_file_uploads($vid) {
    $output = array();
    
    // {upload}
    $query = self::d6_db_select('upload', 'u')
      ->fields('u', array('fid', 'vid', 'description', 'list'))
      ->condition('u.vid', $vid, '=');
    $query->join('files', 'f', 'u.fid = f.fid');
    $query->addField('f', 'filepath');
    $query->addField('f', 'timestamp');
    $query->orderBy('u.fid', 'ASC');
    $result = $query->execute();
    
    foreach ($result as $row) {
      $file = array(
        'path' => variable_get('os_migrate_files', FALSE) . '/' . $row->filepath,
        'description' => $row->description,
        'display' => $row->list,
        'timestamp' => $row->timestamp,
      );
      $output[] = drupal_json_encode($file);
    }
    
    // {content_field_os_inline_files} - filefield
    foreach (array('os_inline_files', 'presentation_file') as $field) {
      $table = 'content_field_' . $field;  
    
      $query = self::d6_db_select($table, 'ff')
        ->fields('ff', array('vid', 'nid', 'delta', 'field_'.$field.'_fid', 'field_'.$field.'_list', 'field_'.$field.'_data'))
        ->condition('ff.vid', $vid, '=');
      $query->join('files', 'f', 'ff.field_'.$field.'_fid = f.fid');
      $query->addField('f', 'filepath');
      $query->addField('f', 'timestamp');
      $query->orderBy('f.fid', 'ASC');
      $result = $query->execute();
      
      foreach ($result as $row) {
        $ff_data = unserialize($row->{'field_'.$field.'_data'});
        $file = array(
          'path' => variable_get('os_migrate_files', FALSE) . '/' . $row->filepath,
          'description' => $ff_data['description'],
          'display' => $row->{'field_'.$field.'_list'},
          'timestamp' => $row->timestamp,
        );
        $output[] = drupal_json_encode($file);
      }
    }
    
    
    return $output;
  }
    
  
}
  

