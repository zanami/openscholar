<?php 

/**
 * @class os_simpleview_list
 * 
 * Box for displaying/configuring os_simpleview_lists 
 */

class os_simpleview_list extends os_boxes_default {
  public function options_defaults() {
    $options = parent::options_defaults();
    $options += $this->_get_list_class()->options_defaults();
    
    return $options;
  }
  
  public function options_form() {
    $form = parent::options_form();
    $form += $this->_get_list_class()->options_form($this->options);

    return $form;
  }
  public function render() {
    return parent::render();
  }
  
  function _get_list_class($class_name = 'os_simpleview_list_default') {
    $plugin = os_simpleview_list_plugin_load($class_name);
    $class = ctools_plugin_get_class($plugin, 'handler');
    return new $class();
  }
  
  //parent::save();
}
