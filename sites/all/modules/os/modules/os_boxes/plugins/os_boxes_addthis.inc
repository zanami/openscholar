<?php

/**
 * @file
 * The class for "CC license" type widgets.  
 */

class os_boxes_addthis extends os_boxes_default {
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    $options['widget_type'] = 'addthis_button';
    $options['toolbox_services'] = 'facebook,twitter,email,linkedin,google_plusone';
    /* @todo other options
     * icon sizes
     * profile id
     */
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){    
    $form = parent::options_form();
    
    $form['widget_type'] = array(
      '#type' => 'radios',
      '#title' => t('Display style'),
      '#default_value' => $this->options['widget_type'],
        '#options' => array(
		  'buttons' => t('Button'), //default
    	  'toolbox_small' => t('Toolbox sm'),//sm
    	  'toolbox_large' => t('Toolbox lg'),//big
    	  'addthis_numeric' => t('Numberic'),
    		
      ),
    );
    
    $form['toolbox_services'] = array(
      '#type' => 'value',
      '#title' => 'Toolbox services',
      '#value' => $this->options['toolbox_services'],
      '#description' => 'A comma separated list of social media links service codes.  (ie: twitter, facebook, google).  For full listing of service codes, please see ' . l('AddThis', 'http://www.addthis.com/services/list#.TlvYUhiwe9E'),
    );

    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    if (! module_exists('addthis')) {
      return;
    }
    //  $toolbox_classes = check_plain(variable_get('addthis_toolbox_classes', ''));

    $vsite = vsite_get_vsite();
    $size = $vsite->controllers->variable->get('addthis_toolbox_classes', 'space');
    
	switch ($this->options['widget_type']) {
		
  	  case 'buttons':
  	  	$services = NULL;
  	  	$config = array();
  	  	$display = 'addthis_button';
  	  	if ($size) {
	  	  $vsite->controllers->variable->del('addthis_toolbox_classes');
  	  	}
  	  	break;
  	  	
  	  case 'toolbox_small':
  	    $services = $this->options['toolbox_services'];
  	    $config = array();
  	    $display = 'addthis_toolbox';
  	    if ($size) {
	  	  $vsite->controllers->variable->del('addthis_toolbox_classes');
  	  	}
  	    break;
  	    
  	  case 'toolbox_large':
  	    $services = $this->options['toolbox_services'];
  	    $config = array();
  	    $display = 'addthis_toolbox';
		if (!$size) {
		  //size is controlled by a var, not by config.  set that var when large icons are used.  reset elsewhere
	  	  $vsite->controllers->variable->set('addthis_toolbox_classes', 'addthis_32x32_style');
  	  	}
  	    break;
  	    
  	}
  	
    $block = parent::render();
    $block['content'] = theme($display, $services, $config);    
    return $block;
  }
  
}
