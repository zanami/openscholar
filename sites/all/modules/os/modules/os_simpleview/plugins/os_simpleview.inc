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

interface os_simpleview {
  
  /**
   * @function options_defaults
   * 
   * Array of defaults for forms defined in this plugin
   */
  function options_defaults();  
  
  
  /**
   * @function multiform_info()
   * 
   * provides form into for ctools_wizard_multistep_form
   */
  function multiform_info();
  
  /**
   * @function render
   * 
   * Executes view.  Gets stored options and passes them to the view
   */
  function render($options = array());
  
  /**
   * @function prepare_view
   * 
   * Applies view overrides to view
   */
  function views_alter(view &$view, &$display_id, &$args, $options) ;
  
  /**
   * @function view_overrides
   * 
   * Provides array of view overrides based on box's options 
   */
  function view_overrides($opts);  

  /**
   * @function view_append
   * 
   * Provides view options to merge with existing options without overwriting (ie filters) 
   */
  function view_append($opts);
  
  /**
   * @function view_settings
   * 
   * Tells simpelview_list which views to use
   * Return an object with view_name and display_id properties to specify a view
   */
  function view_settings();

}