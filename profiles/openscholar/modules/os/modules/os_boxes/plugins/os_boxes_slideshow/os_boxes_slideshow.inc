<?php

/**
 * @file
 * The box class for "Twitter feed" type widgets.
 */

class os_boxes_slideshow extends os_boxes_default {

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    $options['gallery_nid'] = 'all';
    $options['pager'] = 'none';
    $options['controls'] = 'none';
    $options['slide_counter'] = 'none';
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {

    $form = array();
    $form += parent::options_form($form_state);
    
    //get the media gallery nodes. 
    if ($view = views_get_view('gallery_slideshow')) {
      $view->execute();
    }
    
    //jsut get gallery nids and titles  
    $galleries = array();
    foreach ($view->result as $result) {
      if (!isset($galleries[$result->media_gallery_media_file_managed_nid])) {
        $galleries[$result->media_gallery_media_file_managed_nid] = $result->media_gallery_media_file_managed_title;
      }
    }

    $form['gallery_nid'] = array(
      '#type' => 'select',
      '#title' => t('Gallery'),
      '#description' => t('Select a gallery to use as a slideshow.'),
      '#options' => (count($galleries)) ? array('all' => t('All')) + $galleries : array(),
      '#default_value' => $this->options['gallery_nid'],

       //state hide when empty
    );
    
    $form['new_gallery'] = array(
      '#type' => 'markup',
      '#markup' => t('Or create a new gallery'),
    );
    
    $form['display'] = array(
      '#type' => 'fieldset',
      '#title' => t('Display Options'),
      '#collapsible' => TRUE,
    	'#collapsed' => TRUE,
    );
    
    $display_options = array('pager' => t('Show Pager'), 'controls' => t('Show Controls'), 'slide_counter' => t('Slide Counter'));
    foreach ($display_options as $option => $title)
    $form['display'][$option] = array(
      '#type' => 'radios',
      '#title' => $title,
      '#options' => array('none' => t('No Display'), 'top' => t('Top'), 'bottom' => t('Bottom')),
      '#default_value' => $this->options[$option],
    );
    
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $block = parent::render();
    
    if ($view = views_get_view('gallery_slideshow')) {
    
      if (((int)$this->options['gallery_nid']) > 0) {
        $view->set_arguments($this->options['gallery_nid']);
      }
      $view->init_display();
      $handler = $view->display_handler;
      $style_options = $handler->get_option('style_options');
  
      foreach(array('pager', 'controls', 'slide_counter') as $option) {
        foreach($style_options['widgets'] as $position => $info) {
          $style_options['widgets'][$position]['views_slideshow_' . $option]['enable'] = ($position == $this->options[$option]);
        }
      } 
      $handler->set_option('style_options', $style_options);
      
      $output = $view->render();
      $block['content'] = $output;
    }
    
    return $block;
  }

  /**
   * Return the cache parameters for this box
   */
  public function cache_info() {
    
    return array();
    
  //  	//Clear any time the site is saved
  //    $cache_key = array(
  //      'os_og' => array('current' => 'current'),
  //    );
  //
  //    //Only cache twitter feeds for 15 minutes (max_lifespan)
  //    return array('type' => BLOCK_CACHE_CONTENT_CACHE, 'keys' => $cache_key, 'max_lifespan' => 900);
  }

}