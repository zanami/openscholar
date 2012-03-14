<?php 

/**
 * @class scholar_books
 * 
 * Extends os_simpleview_list to provide options for books
 */

class scholar_book extends os_simpleview_list_default {   
  //override the default view
  function view_settings() {
    return (object)array('view_name' => 'scholar_books', 'display_id' => 'block_1');
  }
  
}