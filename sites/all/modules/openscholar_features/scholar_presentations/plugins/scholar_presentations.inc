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
  
  public function multiform_info(){
    $form = parent::multiform_info();

    //appends the presentations form
    $form['forms']['presentations_form'] = array(
    	'form id' => 'os_simpleview_list_form_presentation',  //fill out the form later.  this is just the logic for altering pages
      'title' => t('Presentation'),
      'class' => get_class($this),
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
}