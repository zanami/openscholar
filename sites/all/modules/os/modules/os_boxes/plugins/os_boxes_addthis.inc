<?php

/**
 * @file
 * The class for "CC license" type widgets.
 */

class os_boxes_addthis extends os_boxes_default {

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults(){
    $options = parent::options_defaults();
    $options['widget_type'] = 'buttons';
    $options['toolbox_services'] = 'facebook,twitter,email,linkedin,google_plusone';
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
    
    global $base_url;
    $dir = $base_url . '/' . $mod = drupal_get_path('module', 'os_boxes') . '/theme/images/';
    
    $form['widget_type'] = array(
      '#type' => 'radios', 
      '#title' => t('Display style'), 
      '#default_value' => $this->options['widget_type'], 
      '#options' => array(
        'buttons' => "<img src='$dir/addthis_button.png' />\n", 
        'toolbox_small' => "<img src='$dir/addthis_sm.png' />\n", 
        'toolbox_large' => "<img src='$dir/addthis_lg.png' />\n", 
        'numeric' => "<img src='$dir/addthis_numbers.gif' />\n", 
        'counter' => 'counter' 
      ) 
    );
    
    $form['toolbox_services'] = array(
      '#type' => 'value', 
      '#title' => 'Toolbox services', 
      '#value' => $this->options['toolbox_services'], 
      '#description' => 'A comma separated list of social media links service codes.  (ie: twitter, facebook, google).  For full listing of service codes, please see ' . l('AddThis', 'http://www.addthis.com/services/list#.TlvYUhiwe9E') 
    );
    
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render(){
    if (! module_exists('addthis')) {
      return;
    }
    
    switch ($this->options['widget_type']) {
      
      case 'buttons' :
        $services = NULL;
        $config = array();
        $display = 'addthis_button';
        break;
      
      case 'toolbox_small' :
        //$services = $this->options['toolbox_services'];
        $services = 'facebook,twitter,email,linkedin, more';
        $config = array();
        $display = 'addthis_toolbox';
        break;
      
      case 'toolbox_large' :
        //$services = $this->options['toolbox_services'];
        $services = 'facebook,twitter,email,linkedin, more';
        $config = array(
          'addthis_toolbox_classes' => 'addthis_32x32_style' 
        );
        $display = 'addthis_toolbox';
        break;
      
      case 'numeric' :
        $services = 'facebook_like_counter, tweet, google_plusone, counter';
        $config = array(
          'addthis_toolbox_classes' => 'addthis_pill_combo' 
        );
        $display = 'addthis_toolbox';
        break;
      
      case 'counter' :
        $services = 'counter, addthis_bubble_style';
        $config = array(
          'addthis_toolbox_classes' => 'addthis_bubble_style' 
        );
        $display = 'addthis_toolbox';
        break;
    
    }
    
    $block = parent::render();
    $block['content'] = theme($display, $services, $config);
    return $block;
  }

}
