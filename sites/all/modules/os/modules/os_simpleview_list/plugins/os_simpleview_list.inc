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
        $output = $view->execute_display();
        
        if ($options['more']) {
          $output .= l($options['more_text'], $options['more_link']);
        }
        
        return $output;
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
   * @function _parse_option_OPTION
	 * 
	 * helper functions for plugins that have overrides to build
	 * provides some common options that can be easily overwritten/ignored
   *
   */
  protected  function _parse_options_sorts($sorting_method) {
    $overrides = array('sorts' => array());
    switch ($sorting_method) {
      case 'newest':
        $overrides['sorts']['created'] =  array(
          'order' => 'DESC',
          'granularity' => 'second',
          'id' => 'created',
          'table' => 'node',
          'field' => 'created',
          'relationship' => 'none',
        );
      break;
      
      case 'oldest':
        $overrides['sorts']['created'] =  array(
          'order' => 'ASC',
          'granularity' => 'second',
          'id' => 'created',
          'table' => 'node',
          'field' => 'created',
          'relationship' => 'none',
        );
      break;

      case 'alpha':
        $overrides['sorts']['title'] = array(
          'order' => 'ASC',
          'id' => 'title',
          'table' => 'node',
          'field' => 'title',
          'relationship' => 'none',
        );
      break;
    }
    
    return $overrides;
  }
  
  protected function _parse_options_row_options($display_style) {
    $overrides= array();
    
    switch ($display_style) {
      case 'titles':
        //$row_options = array('teaser' => 0, 'links' => 1, 'comments' => 0);
        $overrides['fields'] = array(
			    'title' => array(
        	  'label' => '', 	
	          'alter' => array(),	
    	      'empty' => '', 	
	          'hide_empty' => 0,	
	          'empty_zero' => 0,	
	          'link_to_node' => TRUE,	
	          'spaces' => array('frontpage' => 0, 'type' => 'spaces_og'),	
	          'exclude' => 0,	
	          'id' => 'title',	
						'table'	=> 'node',
	          'field'	=> 'title',
						'relationship' => 'none'	
          ),
		    );
		    $overrides['style_plugin'] = 'list';
		    $overrides['style_options'] = array('grouping' => '', 'type' => 'ul');

      break;
      /*
      case 'full':
      break;
      */
      case 'teaser':     
      default:
      break;
    }
    
    return $overrides;    
  }
  /*
  protected  function  _parse_options_more($opts) {
    $overrides = array();
    
    if ($opts['more']) {
      $overrides['use_more'] = TRUE;
      $overrides['use_more_text'] = $opts['more_text'];
      $overrides['link_display'] = $opts['more_link'];
      //more_link isn't part of views.  render_more_link builds the path automatically.  our more link from other boxes
      //doesn't act like the views one.  maybe it should be drawn after instead if we're to keep the path overriiding      
    }
    
    return $overrides;
  }*/
}