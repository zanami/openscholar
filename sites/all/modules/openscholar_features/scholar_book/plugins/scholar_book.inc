<?php 

/**
 * @class scholar_books
 * 
 * Extends os_simpleview to provide options for books
 */

class scholar_book extends os_simpleview_default {   
  //override the default view
  function view_settings() {
    return (object)array('view_name' => 'scholar_books', 'display_id' => 'block_1');
  }
  
  //view is not field based.  make it field based for titles
  protected function _parse_options_row_options($display_style) {
    $override = parent::_parse_options_row_options($display_style);
    if ($display_style == 'titles') {
      $override['row_plugin'] = 'fields';
    }
    return $override;
  } 
}