<?php

/**
 * @file
 * The class for "mailing list" type widgets.
 */

class os_boxes_mailchimp extends os_boxes_default {
  
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    
    $options['api'] = '';
    $options['list'] = '';
    $options['text'] = t('Subscribe to our mailing list');

    return $options;
  }
  
  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){
    $form = array();
    
    $form['subscribe']['api'] = array(
      '#type' => 'textfield',
      '#title' => t('Your MailChimp API key'),
      '#default_value' => $this->options['api'],
      '#description' => '<i>Link to MC account</i>  Example: 1234567890abcefghijklmnopqrstuvwx-us2',
    );
    
    $form['subscribe']['list'] = array(
      '#type' => 'textfield',
      '#title' => t('Mailing List'),
      '#default_value' => $this->options['list'],
      '#description' => t('Name of the mailing list to add users to.'),
    );
    
    $defaults = $this->options_defaults();
    $form['subscribe']['text'] = array(
      '#type' => 'textfield',
      '#title' => 'Subscribe Text',
      '#default_value' => $this->options['text'],
      '#description' => t('Text for subscribe link.  Default: ' . $defaults['text'] . '.'), 
    );
    
    return $form;
  }
  
  /**
   * Implementation of boxes_content::render().
   */
    
  public function render() {
    $block = parent::render();
    $form = array();
    global $user;
    //if (isset $user->name) try for fn/ln.  no idea where they're being held.
    
    /* prepopulate form if possible.  logged in users will submit immediately, others will have to fill out form */
    $form['subscribe']['email'] = array(
      '#type' =>  'textfield',
      '#title' => 'Email Address',
      '#value' => ($user->uid != 0) ? $user->mail : '',
      '#weight' => -10,
    );
    
    $form['subscribe']['fn'] = array(
      '#type' =>  'textfield',
      '#title' => 'First Name',
    	'#weight' => -8,
    );
    
    $form['subscribe']['ln'] = array(
      '#type' =>  'textfield',
      '#title' => 'Last Name',
    	'#weight' => -6,
    );
    
    $form['subscribe']['api'] = array('#type' => 'value', '#value' => $this->options['api']);
    
    $form['subscribe']['list'] = array('#type' => 'value', '#value' => $this->options['list']);
    
    $form['subscribe']['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Subscribe',
      '#ahah' => array(                    //ahah why you no work?
        'path' => 'os_mailchimp/subscribe',
      	'method' => 'replace',      
      ),
      '#weight' => -4,
    );
    
    
    // move this to a theme function at some point.
    $block['content']  = '<div id="mailchimp_sub">' . $this->options['text'] . '</div>';
    $block['content'] .= '<div id="mailchimp_form">' . drupal_render($form) . '</div>';    //or should this form be a modal?
    drupal_add_css($this->css, 'theme');
    drupal_add_js('os_mailchimp.js', 'file');
    
    return $block;
  }
}