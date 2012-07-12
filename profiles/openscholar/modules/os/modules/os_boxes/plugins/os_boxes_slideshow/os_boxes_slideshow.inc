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
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {

    $form = array();
      
    // get gallery nids
    $view = views_get_view('cp_content'); //we shouldn't depend on cp here.  new view?  or entityfieldquery
    $view->set_display('page_1'); //block 1 showed all content
    $view->init_display();
    $handler = $view->display_handler;
    $filters = $handler->get_option('filters');
    $filters['type']['value'] = array('media_gallery' => 'media_gallery');
    $filters = $handler->set_option('filters', $filters);
    $view->execute();
    
    $galleries = array();
    foreach ($view->result as $result) {
      $galleries[$result->nid] = $result->node_title;
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
    
    $form += parent::options_form($form_state);
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $block = parent::render();
    
    $view = views_get_view('gallery_slideshow');
    $view = views_get_view('g'); //wtf views?  why you rename my view
  
    if (((int)$this->options['gallery_nid']) > 0) {
      $view->set_arguments($this->options['gallery_nid']);
    }
    
   
    
    //filter out images here
    $output = $view->render();
    
    $block['content'] = $output;
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