<?php 

/**
 * @class scholar_presentations
 * 
 * Extends os_simpleview_list to provide options for presentations
 */

class scholar_presentations extends os_simpleview_list_default {
  
  // could the constructor from this take the object from the first pass of the form as a parameter?  
  // that would let us keep the data, but 'swap' classes.  probably better way out there tho.
  public function options_defaults(){
    $options = parent::options_defaults();
    return $options;
  }
  
  public function options_form(){
    $options_form = parent::options_form();
    return $options_form;
  }
  
  public function render(){
    return parent::render();
  }
}
