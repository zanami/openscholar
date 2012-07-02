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
    
    // the default value for the form
    $options['twitter_username'] = '';
    $options['num_items'] = 5;
    $options['followme_link'] = 0;
    $options['followme_text'] = t("Follow me on twitter");
    $options['followme_url'] = '';
    $options['twitter_type'] = 'Username';
    $options['image_fids'] = implode(',', array()); //comma separated array for 
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    drupal_add_css(drupal_get_path('module', 'os_boxes') . '/plugins/os_boxes_slideshow/os_boxes_slideshow.css');
    $form = array();
    
    $view = views_get_view('media_default');
    $view->init_display();
    $handler = $view->display_handler;
    $filters = $handler->get_option('filters');
    
    //hide filters - when do sort and order get added?
    foreach ( $filters as $name => $filter ) {
      $filters[$name]['exposed'] = FALSE;
      unset($filters[$name]['expose']);
    }
    //images only plz
    $filters['type']['value'] = array('image');
    $filters = $handler->set_option('filters', $filters);
    
    $form['media_browser_fieldset'] = array(
    	'#type' => 'fieldset', 
    	'#title' => t('Your uploaded images'), 
    	'#collapsed' => FALSE,
 //   	'#attached' => array('library' => array(array('media', 'media_base'))),
    );
    $form['media_browser_fieldset']['media_browser'] = array(
    	'#markup' => $view->render(),
    );
    
    //states may be tricky.  another hidden element to toggle them?
    $form['add_selected_image'] = array(
      '#type' => 'button',
      '#value' => t('Add Image'),
      '#executes_submit_callback' => FALSE,
    	'#attributes' => array('onclick' => 'return (false);'),
      '#states' => array(), //show for unselected images
    );
    $form['remove_selected_image'] = array(
      '#type' => 'button',
      '#value' => t('Remove Image'),
    	'#attributes' => array('onclick' => 'return (false);'),
    	'#states' => array(), //show for selected images
    );
    
    //array of fids to use.  managed by javascript :-\
    $form['image_fids'] = array(
      '#type' => 'textfield', //'hidden',
      '#default_value'=> $this->options['image_fids'], //json  
    );

    $form['images_fieldset'] = array(
      '#type' => 'fieldset', 
    	'#title' => t('Slideshow Images'), 
    	'#collapsed' => FALSE,   
      '#attached' => array('js' => array(drupal_get_path('module', 'os_boxes') . '/plugins/os_boxes_slideshow/os_boxes_slideshow.js')),
    );
    
    $form['images_fieldset']['images'] = array(
    );
    
    
    $form += parent::options_form($form_state);
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $block = parent::render();
    
    $view = views_get_view('slideshow');
    $view->init_display();
    $handler = $view->display_handler;
    
    $filters = $handler->get_option('filters');
    //view has file id filter.  uses regex to match any of our selected ids
    $this->options['image_fids'] = array(1,3); //forms will save as a string.  how does javascript write an array?  this might get ugly..
    $filters['fid']['value'] = implode('|', $this->options['image_fids']);
    $filters = $handler->set_option('filters', $filters);
    
    
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