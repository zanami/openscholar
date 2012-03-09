<?php 

/**
 * @class scholar_presentations
 * 
 * Extends os_simpleview_list to provide options for presentations
 */

class scholar_presentations extends os_simpleview_list {
  
  // could the constructor from this take the object from the first pass of the form as a parameter?  
  // that would let us keep the data, but 'swap' classes.  probably better way out there tho.
  public function options_defaults(){
    //$options = parent::options_defaults();
    $options = array();
    $options['content_type'] = 'presentation';
    return $options;
  }
  
  public function options_form(){
    $options_form = parent::options_form();
    return $options_form;
  }
  
  /* doesn't happen.  generic box calls render.  maybe it should send it up the chain instead?  
  public function render(){
    return parent::render();
  }
  */
  
  public function views_alter(&$view, &$display_id, &$args, &$options = array()){
    parent::views_alter($view, $display_id, $args, $options);
    //any presentation specific alterations go here
  }
  
  public function multiform_info(){
    $form = parent::multiform_info();

    //appends the presentations form
    $form['presentations_form'] = array(
    	'form id' => 'os_simpleview_list_form_presentation',  //fill out the form later.  this is just the logic for altering pages
      'title' => t('Presentation'),
//      'class' => get_class($this), //obviated by class loop in module
    );
      
    //also add a form that modifies existing.  instead of changing the id, change the class to this one so it can call parent
    
    return $form;
  }

  function os_simpleview_list_form_presentation() {
    //this form is specific to presentations and has been appended to the regular form.  no need to modify parent's form
    $form = array(
      'presentation' => array(
        '#title' => 'I am a presentation',
        '#type' => 'textfield',
      ),
    );
    
    return $form;
  }
  
  protected function view_overrides($opts) {
    //should all this be in an inc file as describe?
     
    //$overrides = parent::view_overrides($opts);
    //sorting
    $overrides = array(); //custom view, so lets not inherit any overrides
    
    $overrides['filters'] = array(
      'type' => array(
        'operator' => 'in',
        'value' => array('presentation' => 'presentation'),
        'group' => 0,
        'exposed' => FALSE,
        'expose' => array('operator' => FALSE, 'label' => ''),
        'id' => 'type',
        'table' => 'node',
        'field' => 'type',
        'relationship' => 'none',
      ),
    );
  
    
    return $overrides;
  }
  
  function view_settings() {
    return (object)array('view_name' => 'os_simpleview_list_base', 'display_id' => 'default');
  }
}