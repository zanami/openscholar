<?php

/**
 * @file
 * The class for "Gallery" type widgets.
 */

class os_boxes_gallery extends os_boxes_default {
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
      '#options' => array(),
      '#required' => TRUE,
    );
    
    // for each gallery node, add nid/node_title to the select options
    $view = views_get_view("scholar_image_gallery");
    $view->set_display('page_1'); //block 1 showed all content
    $view->execute();
    
    if (count($view->result)) {
      //gallery exists, add options
      $form['gallery']['#options'] += array('all' => t('All'));
      
      foreach($view->result as $r) {
        $form['gallery']['#options'] += array($r->nid => $r->node_title);
      }
      $form['gallery']['#default_value'] = $this->options['gallery'];
    } else {
      //no galleries to display
      $form['gallery']['#description'] = t('No galleries found.');
      $form['gallery']['#disabled'] = TRUE;
      
    }
        
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
    $view = views_get_view('scholar_image_gallery');
    $view->set_display('block_1');
    $view->display_handler->set_option('items_per_page', (int)$this->options['number_of_items']);
      
    if ($this->options['gallery'] != 'all') {
      $view->set_arguments(array($this->options['gallery']));
    }
      
    $block = parent::render();
    $output = $view->execute_display();
    if(is_array($output) && isset($output['content'])){
      $block['content'] = $output['content'];
    }
    return $block;
  }
  
  /**
   * Return the cache key for image view
   */
  public function cache_keys(){
  	$cache_key = parent::cache_keys();
  	
  	//Clear the cache whenever a image or gallery is saved
  	$cache_key['node'] = array('image' => 'image', 'gallery' => 'gallery');
  	
  	return $cache_key;
  }

}
