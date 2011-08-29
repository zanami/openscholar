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
    $options['toolbox_services'] = 'twitter,google_plusone';
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
		  'addthis_button' => t('Button'),
    	  'addthis_toolbox' => t('Toolbox'),
      ),
    );
    
    $form['toolbox_services'] = array(
      '#type' => 'textfield',
      '#title' => 'Toolbox services',
      '#default_value' => $this->options['toolbox_services'],
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
    
  	$addthis_widget_type = $this->options['widget_type'];
  	switch ($addthis_widget_type) {
  	  case 'addthis_button':
  	  	$services = NULL;
  	  	$config = array();
  	  	break;
  	  case 'addthis_toolbox':
  	    $services = $this->options['toolbox_services'];
  	    $config = array();
  	    break;
  	  case 'addthis_toolbox_item':
  	  	$services = NULL;
  	  	$config = array();
  	  	break;
  	}
  	
    $block = parent::render();
    $block['content'] = theme($addthis_widget_type, $services, $config);    
    return $block;
  }
  
}
