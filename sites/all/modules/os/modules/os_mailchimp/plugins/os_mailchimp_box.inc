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
      '#description' => 'Your <a href="https://us1.admin.mailchimp.com/account/api-key-popup" target="_new">MailChimp API key</a>.  Example: 1234567890abcefghijklmnopqrstuvwx-us2',
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
    
    
    // move this to a theme function at some point.
    $block['content']  = '<div id="mailchimp_sub">' . $this->options['text'] . '</div>';
    //$block['content'] .= '<div id="mailchimp_form">' . drupal_get_form('os_mailchimp_form', 'api', 'list') . '</div>';    //or should this form be a modal?
    
    //lets try a modal instead.
    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'os_mailchimp') .'/os_mailchimp.js');
    //store box delta, so recieving script can get api key.
    drupal_add_js(array('os_mailchimp'=>array('delta'=>$this->delta)), 'setting'); //what happens if multiple forms are present?
    $block['content'] =   l($this->options['text'], 'os_mailchimp/form', array('attributes'=>array('class'=>'mailchimp-modal')));   
    
    $block['content'] .= '<i>List: ' . $this->options['list'] . '<br />API:' . $this->options['api'] . '</i>'; //just for dev.  rm later
    drupal_add_css($this->css, 'theme');
    //drupal_add_js('os_mailchimp.js', 'file');
    
    return $block;
  }
}