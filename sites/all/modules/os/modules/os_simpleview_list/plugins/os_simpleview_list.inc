<?php 

/**
 * @file
 * 
 * simpleview list abstract class
 * 
 * Provides common methods.
 * Abstract interface for methods and variables plugins should implement
 * Note that this provides options, form, and render, but is not to be called as a box
 **/

abstract class os_simpleview_list {
  
  /**
   * @function options_defaults
   * 
   * Array of defaults for forms defined in this plugin
   */
  abstract public function options_defaults();  
  
  
  /**
   * @function multiform_info()
   * 
   * provides form into for ctools_wizard_multistep_form
   */
  protected function multiform_info() {
    return array();
  }
  
  /**
   * @function render
   * 
   * Executes view.  Gets stored options and passes them to the view
   */
  public function render($options = array()) {
    $view_settings = $this->view_settings();
    if ($view = views_get_view($view_settings->view_name)) {
      if ($view->access($view_settings->display_id)) {
        $view->set_display($view_settings->display_id);
        
        //Tie this view to this box (this tells the view to call this box's view alter)
        $view->dom_id = "boxes-".$this->delta;
        $this->prepare_view($view->display_handler, $options);
        return $view->execute_display();
      }
      $view->destroy();
    }
  }
  
  /**
   * @function prepare_view
   * 
   * Applies view overrides to view
   */
  private function prepare_view(views_plugin_display_default &$handler, $options) {
    foreach($this->view_overrides($options) as $option => $value) {
      /*if (in_array($option, array('sorts'))) {
        $function = 'os_simpleview_list_prepare_view_$option';
        $value = $function($value);  
      }*/
      $handler->override_option($option, $value);
      
    }
  }
  
  /**
   * @function view_overrides
   * 
   * Provides array of view overrides based on box's options 
   */
  abstract protected function view_overrides($opts);  

  /**
   * @dunction view_settings
   * 
   * Tells simpelview_list which views to use
   * Return an object with view_name and display_id properties to specify a view
   */
  abstract protected function view_settings();
  
  /*public function default_list_form() {
    return $this->options_form();
  }*/

  /**
   * @function os_simpleview_list_prepare_view_$OPTION
   * 
   * Adds options to view handler, but other plugins can override these default functions
   */
  protected function os_simpleview_list_prepare_view_sorts(&$handler, $settings) {
    
  }
  
  /**
   * @function _parse_option_OPTION
	 * 
	 * helper functions for plugins that have overrides to build
	 * provides some common options that can be easily overwritten/ignored
   *
   */
  protected  function _parse_options_sorts($sorting_method) {
    switch ($sorting_method) {
      case 'newest':
        $sorts =  array(
          'order' => 'DESC',
          'granularity' => 'second',
          'id' => 'created',
          'table' => 'node',
          'field' => 'created',
          'relationship' => 'none',
        );
      break;
      
      case 'oldest':
        $sorts['created'] =  array(
          'order' => 'ASC',
          'granularity' => 'second',
          'id' => 'created',
          'table' => 'node',
          'field' => 'created',
          'relationship' => 'none',
        );
      break;

      case 'a-z':
        $sorts['title'] = array(
          'order' => 'ASC',
          'id' => 'title',
          'table' => 'node',
          'field' => 'title',
          'relationship' => 'none',
        );
      break;
    }
    
    return $sorts;
  }
  
  protected function _parse_options_style($display_style) {
    
  }
}