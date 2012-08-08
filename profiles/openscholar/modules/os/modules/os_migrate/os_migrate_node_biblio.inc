<?php 

/**
 * @file os_migrate_node_biblio.inc
 * 
 */

class BiblioNodeOSMigration extends AbstractNodeOSMigration {
  public function __construct() {
    
    /**
     * AbstractNodeOSMigration's __construct() will set up most of the common node migration. 
     */
    parent::__construct('biblio', 'biblio'); //args are d7 bundle, d6 content type.
    
    $this->description = t('Publications migration.');
    
    
    //$this->addUnmigratedDestinations($fields)
    $this->addUnmigratedDestinations(parent::subfields('field_biblio_image', array('file_class', 'language', 'destination_dir', 'destination_file', 'file_replace', 'preserve_files', 'source_dir')), t('DNM'));
    
    $this->addUnmigratedSources(array('field_biblio_image_fid', 'field_biblio_image_list', 'field_biblio_image_data'), t('DNM'));
   
    $this->addSimpleMappings(array('field_biblio_image', 'field_biblio_image:alt', 'field_biblio_image:title'));
    
    
  }
  
  public function query($content_type) {
    $query = parent::query($content_type);

    /**
     * Modify the query.  Add joins and fields here.  content_type_${content_type} and its fields are already included.
     */
    
    //biblio table
    $query->join('biblio', 'b', 'n.vid = b.vid');
    $query->fields('b');
    
   // parent::addQueryCCK($query, $content_type);  //shouldn't this be in the parent query?  maybe check if cck is provided first
    return $query;
  }
  
  
  public function sourceFields($content_type) {
    $fields = parent::sourceFields($content_type);

    /**
     * Describe each of the source columns
     */
    $fields += array(
      'field_biblio_image' => t('Book cover'),
    	'field_biblio_image:alt' => t('Book cover alt text'),
    	'field_biblio_image:title' => t('Book cover title'),
    );
    
    return $fields;
  }
  
  public function prepareRow($current_row) { 
		
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
    
		if ($current_row->field_biblio_image_fid) {
		  parent::d6_db_select('files', 'f')
		  	->condition('f.fid', $current_row->field_biblio_image_fid, '=')
		  	->fields('f');
		  
		  if ($current_row->field_biblio_image_data && $data = unserialize($current_row->field_biblio_image_data)) {
		    $current_row->{'field_biblio_image:alt'} = isset($data['alt']) ? $data['alt'] : '';
		    $current_row->{'field_biblio_image:title'} = isset($data['title']) ? $data['title'] : '';
		  }
		}
		
		return parent::prepareRow($current_row); //false -> skip this row
		
		
  }
  
  /**
   * @function prepare()
   * 
   * Biblio's fields aren't really fields so migrate isn't aware of them.  But migrate provides
   * us with the prepare function, which lets you modify an entity before it's saved.    
   */
  function prepare($entity, $row) {
    foreach(get_object_vars($row) as $key => $value) {
      if (strpos($key, 'biblio_') === 0 && $row->$key) {
        $entity->$key = $value;
      }
    }
  }
  
}