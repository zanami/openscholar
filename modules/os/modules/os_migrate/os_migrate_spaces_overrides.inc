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
      list($object_id, $value) = $this->_update_context($object_id, $value, $row->id);
      if (!isset($value, $object_id)) {
        return FALSE; //skip this row.
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
  
  /**
   * Update the spaces_features variable by fixing the module names.
   */ 
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
  
  /**
   * @function _update_context
   * 
   * @param string $object_id
   *   identifier of context
   * @param array $value
   *   Array of blocks provided by context
   * @param int $nid
   *   Node id of the vsite providing this group
   *   
   * @return boolean|array
   *   Returns updated $object_id and $value parameters or FALSE.
   */
  public function _update_context($object_id, array $value, $nid) {

    //Update the key to use the new context name.  Or bail out to drop the row.
    if (($object_id = $this->_update_context_key($object_id, $nid)) === FALSE) {
      return FALSE;
    }
       
    //boxes in values
    foreach ($value['blocks'] as $id => $block) {
      //update id
      $new_id = $id;
      unset($value['blocks'][$id]);
    
      //and block
      //(02:40:42 PM) Matt Petrovic: context only needs module, delta, region, weight, status and title
      $keep = array('module', 'delta', 'region', 'weight', 'status', 'title');
      $block = array_intersect_key($block, array_flip($keep));
      $block['title'] = '';
      $value['blocks'][$new_id] = $this->_update_block($block);
    }
    
    return array($object_id, $value);
  }
  
  //update a context's key.  return false if we're dropping it.
  public function _update_context_key($object_id, $nid) {
    //old to new mapping
    $contexts = array(
      'openscholar-section-sitewide' => FALSE, //no obvious mapping.  nothing in db
      'openscholar-section-sitewide_front' => 'os_sitewide_front',
      'vsite-section-cp' => 'os_cp',
      'vsite-section-public' => 'os_public',
      'openscholar-feature-image_gallery' => 'gallery_gallery',
      'openscholar-feature-front' => 'os_front',
    );
    
    // In d6 we stored settings per theme.  Not doing that anymore.  Drop all settings that weren't using active theme.
    $og_theme = parent::d6_db_select('og')
    ->fields('og', array('og_theme'))
    ->condition('nid', $nid, '=')
    ->execute()
    ->fetchField();
    
    list($context, $action, $type, $theme) = explode(':', $object_id);
    if ($theme != $og_theme) {
      return FALSE;
    }
    
    //update the context name
    if (isset($contexts[$context])) {
      $context = $contexts[$context];
    } else {
      $context = preg_replace('/openscholar-feature-(\w+)/', '$1_$1', $context); //err?
    }
    
    //$theme = $this->_update_theme_name($theme, FALSE);
    $object_id = implode(':', array($context, $action, $type));
    return $object_id;
  }
  
  //update a block's array
  public function _update_block(array $block) {
    $block['region'] = $this->_update_region($block['region']);
    $block['module'] = $this->_update_module($block['module']);
    //$block['plugin'] almost certainly needs an update    
    return $block;
  }
  
  /**
   * @function _update_module
   * 
   * Updates a module name where the scholar prefix was shortened to os.  Takes array of names or a single string.
   */
  public function _update_module($module) {
    return preg_replace('/^scholar_/', 'os_', $module);
  }
  
  /**
   * @function _update_theme_name()
   * 
   * Removes prefix before a scholar theme name.
   * 
   * @param string $theme
   *   Name of legacy theme
   * @param Bool $remove_deprecated (default: true)
   *   Should themes that are no longer supported be converted to the default or preserved?
   * @return string
   *   D7 name of theme
   */
  public function _update_theme_name($theme, $remove_deprecated = TRUE) {
    //rbates kshepsle and other theme ->flavor vsites will need some extra attention here.
    $theme = preg_replace('/^scholar_/', '', $theme);
    
    if ($remove_deprecated && in_array($theme, array('burroughs', 'collector', 'density'))) {
      $theme = 'cleanblue';
    }
    
    return $theme;
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

