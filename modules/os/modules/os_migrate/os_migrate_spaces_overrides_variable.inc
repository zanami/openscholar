<?php 

class SpacesOverridesVariableMigration extends SpacesOverridesTableMigration {
  
  public function __construct() {
    $this->source_object = $this->dest_object = 'variable';
    parent::__construct();
  }
  
  public function query() {
    $query = parent::query();
    
    return $query;
  }
  
  public function prepareRow(&$row) {
    $value = unserialize($row->value);
    $object_id = $row->object_id;
    
    //flavor variables get new names
    if (preg_match('/_flavor$/', $row->object_id)) {
      //$this->_update_flavors($row);
      list($object_id, $value) = $this->_update_flavors($object_id, $value);
    }
    

    /*
     * pseudo-hook:  $this->_update_TYPE_ID($object_id, $value);
    * always returns array of paramters
    */
    $function = '_update_' . $row->object_type . '_' . $object_id;
    if (method_exists($this, $function)) {
      list($object_id, $value) = $this->$function($object_id, $value);
    }
    
    
    
    $row->value = serialize($value);
    return parent::prepareRow($row);
  }

  /*
   * @function _update_flavors
  *
  * Updates flavor variables and names.
  */
  public function _update_flavors($object_id, array $value) {
    $object_id = preg_replace('/vsite_design_scholar/', 'os_appearance', $object_id);
    $value = preg_replace('/^scholar_/', '', $value);
    return array($object_id, $value);
  }
  

  /**
   * pseudo hooks (for lack of a better term)
   *
   * To save a little space and keep from writing a million checks for object id, I'm using the type
   * and id for the name of a variable function.  _update_TYPE_ID.
   *
   * Takes object_id and value.  Returns array of the same to be set with list().
   */
  
  /**
   * Update the spaces_features variable by fixing the module names.
   */
  public function _update_variable_spaces_features($object_id, array $value) {
    $spaces_features = array_flip( $this->_upate_module(array_flip($value)) );   //should this rename be a function?
    return array($object_id, $value);
  }
  
}

