<?php

/**
 * @file
 * The class for "CC license" type widgets.  
 */

class os_boxes_featuredposts extends os_boxes_default {
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();

    $options['number_of_items'] = 10;
    
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){
    
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
    $view = views_get_view('vsite_featuredposts');
    $view->set_display('block_1');
    $view->display_handler->set_option('items_per_page', $this->options['number_of_items']);
          
    $block = parent::render();
    $output = $view->execute_display();
    $block['content'] = $output['content']; 
    return $block;
  }

}