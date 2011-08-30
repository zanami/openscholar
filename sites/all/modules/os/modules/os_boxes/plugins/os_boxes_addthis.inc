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
    $options['widget_type'] = 'buttons';
    $options['toolbox_services'] = 'facebook,twitter,email,linkedin,google_plusone'; //@todo array
    /* @todo other options
     * profile id
     */
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){    
    $form = parent::options_form();
    
    $tb_sm = "small:<div class='addthis_toolbox addthis_default_style ' ><a class='addthis_toolbox_item addthis_button_facebook'></a><a class='addthis_toolbox_item addthis_button_twitter'></a><a class='addthis_toolbox_item addthis_button_email'></a><a class='addthis_toolbox_item addthis_button_linkedin'></a><a class='addthis_toolbox_item addthis_button_google_plusone'></a></div>";
    
    $form['widget_type'] = array(
      '#type' => 'radios',
      '#title' => t('Display style'),
      '#default_value' => $this->options['widget_type'],
        '#options' => array(
		  'buttons' => t('Button'), //default
    	  'toolbox_small' => $tb_sm,//sm
    	  'toolbox_large' => t('Toolbox lg'),//big
    	  'numeric' => "<img src='http://cache.addthiscdn.com/www/20110825162931/images/gtc-like-tweet-share-plusone.gif' />\n",
    		
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
    
	switch ($this->options['widget_type']) {
		
  	  case 'buttons':
  	  	$services = NULL;
  	  	$config = array();
  	  	$display = 'addthis_button';
  	  	break;
  	  	
  	  case 'toolbox_small':
  	    $services = $this->options['toolbox_services'];
  	    $config = array();
  	    $display = 'addthis_toolbox';
  	    break;
  	    
  	  case 'toolbox_large':
  	    $services = $this->options['toolbox_services'];
  	    $config = array('addthis_toolbox_classes' => 'addthis_32x32_style'); 
  	    $display = 'addthis_toolbox';
  	    break;
  	    
  	  case 'numeric':
  	  	$services = 'facebook_like_counter, tweet, google_plusone, counter_pill';
  	    $config = array();
  	    $display = 'addthis_toolbox';
  	  	break;
  	}
  	
    $block = parent::render();
    $block['content'] = theme($display, $services, $config);    
    return $block;
  }
  
}
