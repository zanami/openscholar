<?php 

/**
 * @class os_simpleview_list
 * 
 * Box for displaying/configuring os_simpleview_lists 
 */

class os_simpleview_list_box extends os_boxes_modalframe {
  
  // Tells boxes how to configure/add this widget
  protected function config_path() {
    if ($this->options['nid']) {        //@TODO
      return 'node/'.$this->options['nid'].'/edit';
    }
    else {
      return 'os_simpleview_list/js';
    }
  }
  
  public function options_defaults() {
    $options = parent::options_defaults();
    //need to know $class in order to get defaults from svlist plugin
    $options += $this->_get_list_class()->options_defaults();
    
    return $options;
  }
  
  /*
  public function options_form() {
   // $form = parent::options_form();
    //$form = $this->_get_list_class()->options_form($this->options); //this is the plugin, not the box list

    $form = array();
    return $form;
  }*/ //handled by config path?
  
  public function render() {
    $block = parent::render();
    
    $view = 'vsite_view_base';
    $display_id = 'default'; //add a block ID?
    
    if ($view = views_get_view($view)) {
      if ($view->access($display_id)) {
        $view->set_display($display_id);
        
        //Tie this view to this box (this tells the view to call this box's view alter)
        $view->dom_id = "boxes-".$this->delta;
        
        //Adjust the view for each of the options set
        foreach($this->options as $opt => $val) {
          $opt;
        }
        

        
        $output = $view->execute_display();
        $block['content'] = $output;
      }
      $view->destroy();
    }
    
    return $block;
  }
  
  /**
   * Called from views_pre_view
	 * Configures view according to options
   */
  public function views_alter(&$view, &$display_id, &$args) {
    //Use content type's class's views_alter.
    $instance = $this->_get_list_class();
    $instance->views_alter($view, $display_id, $args, $this->options);

  }
  
  function _get_list_class() {
    $class_name = ($this->options['content_type'] == 'all') ? 'default' : $this->options['content_type'];
    
    $plugins = os_simpleview_list_plugin_load();
    $plugin = (isset($plugins[$class_name])) ? $plugins[$class_name] : $plugins['default'];
    
    $class = ctools_plugin_get_class($plugin, 'handler');
    return ($class) ? new $class() : FALSE;
  }
  
  //parent::save();
}
