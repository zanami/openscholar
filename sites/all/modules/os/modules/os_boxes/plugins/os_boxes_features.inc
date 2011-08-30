<?php

/**
 * @file
 * The class for "CC license" type widgets.  
 */

class os_boxes_features extends os_boxes_default {
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
    $view = views_get_view('vsite_view_base');
    vsite_include('vsiteapi');
    $vsite = vsite_get_vsite();
    $node_types = vsite_content_types();
    drupal_alter('vsite_content_types', $node_types);

    //add filters for displayable types
    $view->display['default']->display_options['filters']['type'] = array(
      'operator' => 'in',
      'value' => $node_types,
      'group' => 0,
      'exposed' => FALSE,
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    );
      
    $block = parent::render();
    $view->set_display('default');
    $view->display_handler->set_option('items_per_page', $this->options['number_of_items']);
    
    $output = $view->execute_display();
    $block['content'] = $output;//['content']; 
    return $block;
  }

}