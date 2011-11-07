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
    
    $options['list'] = '';
    $options['text'] = t('Subscribe to our mailing list');

    return $options;
  }
  
  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){
    $form = array();
    
    if (($vsite=vsite_get_vsite()) != FALSE) {
      //openscholar space integration
      $api = $vsite->controllers->variable->get('os_mailchimp_api', 'space');
    } else {
      $api = variable_get('os_mailchimp_api', '');
    }
    
    if (!$api) {
      $form['subscribe']['api_warning'] = array(
        '#value' => '<div class="error">You have not set a <strong>MailChimp API key</strong>.  Until you do so, mailing lists will not work.  You can set up an API key in <a href="/'.$vsite->group->purl.'/cp/settings" target="_new">site settings<a>.</div>',
      );
    } else {
      
      $form['subscribe']['list'] = array(
        '#type' => 'select',
        '#title' => t('Mailing List'),
        '#default_value' => $this->options['list'],
        '#options' => _os_mailchimp_get_lists($api),
        '#description' => 'Choose a mailing list to subscribe users to.  If none are available, check your MailChimp API key in <a href="/'.$vsite->group->purl.'/cp/settings" target="_new">site settings<a>.',
      );
      
      $defaults = $this->options_defaults();
      $form['subscribe']['text'] = array(
        '#type' => 'textfield',
        '#title' => 'Subscribe Text',
        '#default_value' => $this->options['text'],
        '#description' => t('Text for subscribe link.  Default: ' . $defaults['text'] . '.'), 
      );
    }
        
    return $form;
  }
  
  /**
   * Implementation of boxes_content::render().
   */
    
  public function render() {
    $block = parent::render();
    global $user;
    if (TRUE || $user->uid == 0) {
      $block['content'] = drupal_get_form('os_mailchimp_form', $this->options['list']);
      
      
      /*
      if (FALSE) { //TODO make this a box setting
        //modal form takes email, name
        modalframe_parent_js();
        drupal_add_js(drupal_get_path('module', 'os_mailchimp') .'/os_mailchimp.js');
  
        //store box delta, so recieving script can get api key.
        drupal_add_js(array('os_mailchimp'=>array('delta'=>$this->delta)), 'setting'); //what happens if multiple forms are present?
        
        $block['content'] =   l($this->options['text'], 'os_mailchimp/form', array('attributes'=>array('class'=>'mailchimp-modal')));   
      } else {
        
        
        //AHAH form
        $block['content'] = drupal_get_form('os_mailchimp_ahah_form', $this->options['list']);
      }
        
    } else {
      //user is logged in.  use name and email.
      $mail = $user->mail;
      $name = $user->name;  
      
    }*/
    
    //$block['content'] .= '<i>List: ' . $this->options['list'] . '<br />API:' . $this->options['api'] . '</i>'; //just for dev.  rm later
    //drupal_add_css($this->css, 'theme');
    //drupal_add_js('os_mailchimp.js', 'file');
    
    return $block;
    }
  }

  
}