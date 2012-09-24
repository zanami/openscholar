<?php

/**
 * @file os_migrate_flag.inc
 */


// class FlagOSMigration extends OSMigration {
//   public function __construct() {

//     parent::__construct();

//     $this->description = t('Flag migration.');
//     $this->source = new MigrateSourceSQL($this->query(), array(), null, array('map_joinable' => FALSE));
//     $this->destination = new MigrateDestinationFlagSimple();
//     $this->map = new MigrateSQLMap(
//       $this->machineName,
//       array(
//       	'fid' => array(
//           'type' => 'int',
//           'not null' => TRUE,
//           'unsigned' => TRUE,
//           'description' => 'Flag ID.',
//         ),
//       ),
//       MigrateDestinationFlagSimple::getKeySchema()
//     );

//   }

//   public function query() {
//     $query = parent::d6_db_select('flags', 'f')
//       ->fields('f');

//     /**
//      * Modify the query.  Add joins and fields here.  content_type_${content_type} and its fields are already included.
//      */

//     return $query;
//   }


//   public function sourceFields($content_type) {
//     $fields = parent::sourceFields($content_type);

//     /**
//      * Describe each of the source columns
//      */

//     return $fields;
//   }

//   public function prepareRow(&$current_row) {

//     /**
//      * This method is called while looping over the results of the query provided earlier.
//      *
//      * This is your chance to munge data.  (See Abstract's filter formats).
//      *
//      * This is also your chance to query additional tables that didn't make sense to join earlier
//      * (for example, see gallery.  d6's gallery had references to image nodes.  d7's gallery uses
//      * file fields for images.  While looping over d6 gallery nodes, prepareRow uses additional queries
//      * to get those image nodes and extract everything needed by file fields.)
//      */

//     return parent::prepareRow($current_row); //false -> skip this row

//   }
// }