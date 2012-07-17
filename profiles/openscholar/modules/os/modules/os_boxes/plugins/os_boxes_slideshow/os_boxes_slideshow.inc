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
    $options['items_per_page'] = 10;
    $options['effect'] = 'fade';
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
      $view->execute_display('default', array());
    }
    
    //jsut get gallery nids and titles  
    $galleries = array();
    $r = $view->result;
    foreach ($view->result as $result) {
      if (!isset($galleries[$result->node_file_usage_nid])) {
        $galleries[$result->node_file_usage_nid] = $result->node_file_usage_title;
      }
    }

    if (count($galleries)) {
      $form['gallery_nid'] = array(
        '#type' => 'select',
        '#title' => t('Gallery'),
        '#description' => t('Select a gallery to use as a slideshow.'),
        '#options' => array('all' => t('All')) + $galleries,
        '#default_value' => $this->options['gallery_nid'],
      );
    } else {    
      $form['no_gallery_message'] = array(
        '#type' => 'markup',
        '#markup' => '<div>' . t('You must create a gallery to use as a slideshow.') . '</div>',
      );
    }
        
    $form['new_gallery'] = array(
      '#type' => 'markup',
      '#markup' => l(t('Create new gallery'), 'node/add/media-gallery'), //overlay?
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
    
    $form['display']['items_per_page'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of items'),
      '#description' => t('Limit to this many images per slideshow.  Use 0 for no limit.'),
      '#default_value' => (int) $this->options['items_per_page'],
    );
    
/*    $state = array();
    $slideshow_form_full = drupal_retrieve_form('views_slideshow_cycle_views_slideshow_slideshow_type_form', $state);
    $form['display']['effects'] = $slideshow_form_full['views_slideshow_cycle']['effects'];*/
    
    //get jquery effects from view (include file hsould arleady be loaded)
    $slideshow_form_full = $state = array();
    @views_slideshow_cycle_views_slideshow_slideshow_type_form($slideshow_form_full, $state, $view);
    $form['display']['effect'] = $slideshow_form_full['views_slideshow_cycle']['effect'];
    $form['display']['effect']['#default_value'] = $this->options['effect'];
    
    
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
  
      //add slideshow widgets
      foreach(array('pager', 'controls', 'slide_counter') as $option) {
        foreach($style_options['widgets'] as $position => $info) {
          $style_options['widgets'][$position]['views_slideshow_' . $option]['enable'] = ($position == $this->options[$option]);
        }
      } 
      
      $style_options['views_slideshow_cycle']['effect'] = $this->options['effect'];
      $handler->set_option('style_options', $style_options);
      
      //number of items
      if ((int)$this->options['items_per_page'] > 0) {
        $handler->set_option('items_per_page', (int)$this->options['items_per_page']);
      }
      
      $output = $view->preview();
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