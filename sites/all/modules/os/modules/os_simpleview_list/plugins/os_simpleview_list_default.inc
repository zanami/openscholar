<?php 

/**
 * @file
 * 
 * simpleview list default class
 * Note that this provides options, form, and render, but is not to be called as a box
 **/

class os_simpleview_list_default implements os_simpleview_list {
  private $view_name = 'os_simpleview_list_base';
  private $display_id = 'default';
  
  function options_defaults(){ 
    $options = array(
      'number_of_items' => 6,
      'more' => 0,
      'more_text' => t('more'),
      'more_link' => "",
      'use_pager' => 1,
      'style' => 'titles',
      'content_type' => 'all',
      'sorts' => 'newest',
    );

    return $options;
  }
  
  
  // since this is separate from box, take its options for defaults
  public function os_simpleview_list_form_basics($options = NULL, $step = NULL){
    $form = array();
    
    $form['description'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#required' => TRUE,
      //'#default_value' => $options['name'],
      '#description' => t('Name this list so it can be saved.'), 
    );
    
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#required' => TRUE,
    	'#description' => t('Title to show to your visitors who view this list.'),
      //'#default_value' => $options['title']
    );
    
    $form['content_type'] = array(
      '#type' => 'select',
      '#title' => t('Content Type'),
      '#options' => array('all' => t('All')) + _os_simpleview_list_content_types(),
      '#required' => TRUE,
    	'#description' => t('Content types to include in this list.'),
      //'#default_value' => $options['content_type']
    );
    
    return $form;
  }
  
  public function os_simpleview_list_form_display_options($options = NULL, $step = NULL){
    $form = array();
     
  	$form['style'] = array(
      '#type' => 'select',
      '#title' => t('Display Style'),
      '#options' => array(
        'teasers' => t('List of teasers'),
        'titles' => t('List of titles'),
  	    //full posts?  might be useful for announcements and events
      ),
      '#required' => TRUE,
      '#default_value' => $options['style']
    );
    
  	$form['number_of_items'] = array(
      '#type' => 'textfield',
      '#title' => 'Number of displayed posts',
      '#default_value' => $options['number_of_items'],
      '#size' => 10
    );
    
    $form['use_pager'] = array(
      '#title' => t('Add pager'),
      '#type' => 'checkbox',
      '#default_value' => $options['use_pager'],
      '#description' => t('Will add a pager at the end of the list')
    );
    
    $form['more'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add "more" link'),
      '#description' => t('This will add a more link to the bottom of this widget, which will link to the page listing of posts'),
      '#default_value' => $options['more'],
      '#tree' => TRUE
    );
    
    $form['more_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#default_value' => $options['more_text'],
      '#description' => t("The title / label of the link."),
      '#process' => array(
        'ctools_dependent_process'
      ),
      '#dependency' => array(
        'edit-more' => array(
          1
        )
      )
    );
    
    $form['more_link'] = array(
      '#type' => 'textfield',
      '#title' => t('Link'),
      '#default_value' => $options['more_link'],
      '#description' => t("The url/path for this link."),
      '#process' => array(
        'ctools_dependent_process'
      ),
      '#dependency' => array(
        'edit-more' => array(
          1
        )
      )
    );
    
    $form['sorts_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => 'Sorting',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    
    $form['sorts_fieldset']['sorts'] = array(
      '#type' => 'select',
      '#options' => array('newest' => t('Newest'), 'oldest' => t('Oldest'), 'alpha' => t('Alphabetical')),
      '#default_value' => $options['sorts'],
    );
    
    return $form;
  }
  

  /**
   * @function multiform_info()
   * 
   * provides form into for ctools_wizard_multistep_form
   */
  function multiform_info() {    
    return array(      
    	'basics' => array(
      	'form id' => 'os_simpleview_list_form_basics', 
      	'title' => 'The Basics',   
      ),
      'display_options' => array(
      	'form id' => 'os_simpleview_list_form_display_options', 
      	'title' => 'Display Options',
      ),
    );
  }
  
  public function render($options = array()) {
    $view_settings = $this->view_settings();
    if ($view = views_get_view($view_settings->view_name)) {
      if ($view->access($view_settings->display_id)) {
        $view->set_display($view_settings->display_id);
        
        //Tie this view to this box (this tells the view to call this box's view alter)
        $view->dom_id = "boxes-" . $options['delta'];
        $output = $view->execute_display();
        
        //Append more link
        if ($options['more']) {
          $more .= l($options['more_text'], $options['more_link']);
          if (is_array($output)) {
            $output['content'] .= $more;
          } else {
            $output .= $more;
          }
        }
        
        return $output;
      }
      $view->destroy();
    }
  }

  public function views_alter(view &$view, &$display_id, &$args, $options) {
    foreach($this->view_overrides($options) as $option => $value) {
      $view->display_handler->override_option($option, $value);      
    }
    
    foreach($this->view_append($options) as $option => $value) {
      $current = $view->display_handler->get_option($option);
      $view->display_handler->override_option($option, array_merge($current, $value));
    }
  }
  
  /**
   * @function view_overrides
   * 
   * Provides array of view overrides based on box's options 
   */
  function view_overrides($opts) {
    //should all this be in an inc file as describe?  maybe in abstract class?

    //scalar options
    $overrides = array(
      'items_per_page' => intval($opts['number_of_items']), 
      'use_pager' => ($opts['use_pager']) ? 'mini' : FALSE,
    	'use_ajax'  => ($opts['use_pager']) ? TRUE   : FALSE,
    );

    //sorting
    $overrides += $this->_parse_options_sorts($opts['sorts']);
     
    //display - teaser or title
    $overrides += $this->_parse_options_row_options($opts['style']);
        
    return $overrides;
  }
  
  /**
   * as overrides, but these options will merge with existing arrays before being set.
   * Enter description here ...
   * @param unknown_type $opts
   */
  function view_append($opts) {
    $overrides = array();
    
    //content type filter
    $overrides += $this->_parse_options_content_type($opts['content_type']); 
  }
/**
 * Enter description here ...
 * @param opts
 * @param overrides
 */
  protected function _parse_options_content_type($content_type) {
    //content type filter
    $overrides = array();
    if ($content_type != 'all') {    
      $overrides['filters'] = array(
        'type' => array(
          'operator' => 'in',
          'value' => array($content_type => $content_type),
          'group' => 0,
          'exposed' => FALSE,
          'expose' => array('operator' => FALSE, 'label' => ''),
          'id' => 'type',
          'table' => 'node',
          'field' => 'type',
          'relationship' => 'none',
        ),
      );
    }
    return $overrides;
  }
  
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
  
  function view_settings() {
    return (object)array('view_name' => 'os_simpleview_list_base', 'display_id' => 'default');
  } 
}