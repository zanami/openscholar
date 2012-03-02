<?php 

/**
 * @file
 * 
 * simpleview list default class
 * Note that this provides options, form, and render, but is not to be called as a box
 **/

class os_simpleview_list_default {
  public function options_defaults(){
    //$options = parent::options_defaults();

    $options = array();
    $options['number_of_items'] = 6;
    $options['more'] = 0;
    $options['more_text'] = t('more');
    $options['more_link'] = "";
    $options['use_pager'] = 1;
    $options['style'] = 'titles';

    return $options;
  }
  
  
  //how to split options over multi page wizard form?
  // since this is separate from box, take its options for defaults
  public function os_simpleview_list_form_basics($options = NULL, $step = NULL){
  
//    $multi_info = $this->multiform_info();
  //  $form = ctools_wizard_multistep_form($multi_info, $step, $form_state);
//ctools_wizard_multistep_form() returns html.  not quite ready for that yet.
//ctools_wizard_wrapper($form, $form_state) 
  
  	ctools_include('dependent');
    ctools_add_js('dependent');

    $form = array();
    
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#required' => TRUE,
      '#default_value' => $options['name']
    );
    
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#required' => TRUE,
      '#default_value' => $options['title']
    );
    
    $form['content_type'] = array(
      '#type' => 'select',
      '#title' => t('Content Type'),
      '#options' => array('all' => t('All')) + $this->list_content_types(),
      '#required' => TRUE,
      '#default_value' => $options['content_type']
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
    

    return $form;
  }
  
  /**
   * @function multiform_info()
   * 
   * provides form into for ctools_wizard_multistep_form
   */
  public function multiform_info($js = NULL) {
    ctools_include('wizard');    
    return array(
      'id' => 'os_simpleviews_list', 
      'path' => 'os_simpleview_list/' . ($js ? 'ajax' : 'nojs') . '/%step', //pass the step we're on to the form, step1, step2, ..etc
      'show trail' => TRUE,
      'show back' => TRUE,
      'show return' => FALSE,
      'next callback' => 'os_simpleview_list_multiform_next',
      'finish callback' => 'os_simpleview_list_multiform_finish',
      'cancel callback' => 'os_simpleview_list_multiform_cancel', 
      
      'forms' => array(
        
      	'basics' => array(
        	'form id' => 'os_simpleview_list_form_basics', 
        	'title' => 'The Basics',
          'class' => get_class($this), //pass back the class so it can be loaded to get the form    
        ),
        
        'display_options' => array(
        	'form id' => 'os_simpleview_list_form_display_options', 
        	'title' => 'Display Options',
          'class' => get_class($this), //pass back the class so it can be loaded to get the form
        ),
        
//        'obj_form' => array('form id' => 'default_list_form', 'title' => 'content_type_form'),
//        'step2' => array('form id' => 'os_simpleview_list_form2', 'title' => 'Quest'),
//        'step3' => array('form id' => 'os_simpleview_list_form3', 'title' => 'Color'),
    
      )
    );
  }
  
  public function render() {
    $view_name = 'vsite_view_base';  
  
    if ($view = views_get_view($view_name)) {
      true;
    }
  }
  
  public function default_list_form() {
    return $this->options_form();
  }
  
  /**
   * returns a list of content types and their names
   */
  private function list_content_types() {
    // os/vsite integration - lists correct content types
    if (module_exists('vsite') && vsite_get_vsite()) {
      vsite_include('vsiteapi');
      $node_options = vsite_content_types(array(1,2));       //Get public and private types
      drupal_alter('vsite_content_types', $node_options);
    }
    
    if (! isset($node_options)) {
      $node_options = node_get_types('names');
    }
    
    return $node_options;
  }
  
  
    
}