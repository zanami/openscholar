<?php

/**
 * @file
 * The class for "Slideshow" type widgets.
 */

class os_boxes_slideshow extends os_boxes_default {
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();

    $options['gallery'] = 'all';
    $options['number_of_items'] = 10;
    
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){
    
    $form['gallery'] = array(
      '#type' => 'select',
      '#title' => t('Gallery'),
      '#options' => array(
        'all' => t('All')
      ),
      '#required' => TRUE,
    );
    
    // for each gallery node, add nid/node_title to the select options
    $view = views_get_view("scholar_image_gallery");
    $view->set_display('page_1'); //block 1 showed all content
    $view->execute();
    foreach($view->result as $r) {
      $form['gallery']['#options'] += array($r->nid => $r->node_title);
    }
    $form['gallery']['#default_value'] = $this->options['gallery'];
        
    $form['number_of_items'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of items to display'),
      '#default_value' => $this->options['number_of_items']
    );

    $form += parent::options_form();
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    //@todo
    $view = views_get_view('os_slideshow');
    $view->set_display('default');
    $view->display_handler->set_option('items_per_page', (int)$this->options['number_of_items']);
      
    if ($this->options['gallery'] != 'all') {
      $view->set_arguments(array($this->options['gallery']));
    }
      
    $block = parent::render();
    $output = $view->execute_display();
    $block['content'] = $output;
    return $block;
  }
  
  /**
   * Return the cache key for gallery view
   */
  public function cache_keys(){
    $cache_key = parent::cache_keys();
    
    //Clear the cache whenever a image or gallery is saved
    $cache_key['node'] = array('image' => 'image', 'gallery' => 'gallery');
    
    return $cache_key;
  }

}