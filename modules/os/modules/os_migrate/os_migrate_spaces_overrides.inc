<?php

/**
 * @file 
 * Implements TableCopyMigration for each of the biblio tables we're copying.
 */

 abstract class SpacesOverridesTableMigration extends TableCopyMigration {
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
    $query->condition('object_type', $this->source_object, '='); 
    return $query;
  }
  
//   public function prepareRow(&$row) {
//     //return if no processing?  add that later for speed?
//     $value = unserialize($row->value);
//     $object_id = $row->object_id;
    


    
//     $row->value = serialize($value);
    
//   }


  
  //update a block's array
  public function _update_block(array $block) {
    if (isset($block['region'])) {
      $block['region'] = $this->_update_region($block['region']);
    }
    
    if (isset($block['module'])) {
      $block['module'] = $this->_update_module($block['module']);
    }
    
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

