<?php 

/**
 * @file os_migrate_node_event.inc
 * 
 */

class EventNodeOSMigration extends AbstractNodeOSMigration {
  public function __construct() {

    
    /**
     * AbstractNodeOSMigration's __construct() will set up most of the common node migration. 
     */
    parent::__construct('event', 'event'); //args are d7 bundle, d6 content type.
    
    $this->description = t('Calendar events.');
    
    $this->addFieldMapping('field_date', 'field_date_value');
    
  }
  
  public function query($content_type) {
    $query = parent::query($content_type);

    /**
     * Modify the query.  Add joins and fields here.
     */

    return $query;
  }
  

  public function prepareRow($current_row) {
    parent::prepareRow($current_row); 

    /**
     * This method is called while looping over the results of the query provided earlier.
     * 
     * This is your chance to munge data.  (See Abstract's filter formats).
     * 
     * This is also your chance to query additional tables that didn't make sense to join earlier
     * (for example, see gallery.  d6's gallery had references to image nodes.  d7's gallery uses
     * file fields for images.  While looping over d6 gallery nodes, prepareRow uses additional queries
     * to get those image nodes and extract everything needed by file fields.)
     */
    
    //again, shouldn't this live in parent?
    //parent::prepareRowCCK($current_row); //handle listed cck fields.
  }
}