<?php

/**
 * @file 
 * Implements TableCopyMigration for each of the biblio tables we're copying.
 */

class SpacesOverridesTableMigration extends TableCopyMigration {
  public function __construct() {
    $key_schema = array(
        'type' => array(
            'type' => 'varchar',
            'length' => 64,
            'not null' => TRUE,
        ),
        'id' => array(
            'type' => 'varchar',
            'length' => 255,
            'not null' => TRUE,
        ),
        'object_type' => array(
            'type' => 'varchar',
            'length' => 64,
            'not null' => TRUE,
        ),
        'object_id' => array(
            'type' => 'varchar',
            'length' => 255,
            'not null' => TRUE,
        ),
    );

    $table = 'spaces_overrides';

    parent::__construct($table, $key_schema);
  }
  
  public function query() {
    $query = parent::query();
    $query->condition('object_type', 'context', '='); //just for debuggin
    return $query;
  }
  
  public function prepareRow(&$row) {
    //return if no processing?  add that later for speed?
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
    
    
    if ($row->object_type == 'context') {
      //object id has a name change
      
      //boxes in values
      foreach ($value['blocks'] as $id => $block) {
        //update id
        $new_id = $id;
        unset($value['blocks'][$id]);
        
        //and block
        $value['blocks'][$new_id] = $this->_update_block($block);
      }
    }
    
    $row->value = serialize($value);
    
  }
  
  /**
   * pseudo hooks (for lack of a better term)
   * 
   * To save a little space and keep from writing a million checks for object id, I'm using the type
   * and id for the name of a variable function.  _update_TYPE_ID.
   * 
   * Takes object_id and value.  Returns array of the same to be set with list().
   */
  
  //replace scholar with os
  public function _update_variable_spaces_features($object_id, array $value) {
    $spaces_features = array_flip( $this->_upate_module(array_flip($value)) );   //should this rename be a function?
    return array($object_id, $value);
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
  
  //update a block's array
  public function _update_block(array $block) {
    //has the following: module, delta, weight, region, status, label, type.
    $block['region'] = $this->_update_region($block['region']);
    $block['module'] = $this->_update_module($block['module']);
    //$block['plugin'] almost certainly needs an update
    return $block;
  }
  
  //scholar_ -> os_.  thankfully preg_replace takes string or array, I think.
  public function _update_module($module) {
    return preg_replace('/^scholar_/', 'os_', $module);
  }
  
  //return new region name
  public function _update_region($region) {
    $regions = array(
      'header_main' => 'header_second',
      'header_left' => 'header_first',
      'header_right' => 'header_third',
      'navbar' => 'menu_bar',
      'content_top' => 'content_top',
      'content_left' => 'content_first',
      'content_right' => 'content_second',
      'content_bottom' => 'content_bottom',
      'content' => 'content',
      'left' => 'sidebar_first',
      'right' => 'sidebar_second',
      'footer' => 'footer',
    );
    return isset($regions[$region]) ? $regions[$region] : FALSE;
  }
}

