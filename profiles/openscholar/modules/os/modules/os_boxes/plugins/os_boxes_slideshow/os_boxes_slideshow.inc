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
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
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
    );
    $form['media_browser_fieldset']['media_browser'] = array(
    	'#markup' => $view->render()
    );
    media_attach_browser_js($form['media_browser_fieldset']);
    require_once(drupal_get_path('module', 'media') . '/wysiwyg_plugins/media.inc');
    media_include_browser_js();
 //   $form['media_browser_fieldset']['#attached']['js'][] = drupal_get_path('module', 'media') . '/js/plugins/media.library.js';
    

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