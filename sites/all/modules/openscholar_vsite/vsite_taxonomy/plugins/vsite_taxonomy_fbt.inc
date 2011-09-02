<?php

/**
 * @author rbran100
 * @category boxes
 *
 * The class for filtering content by taxonomy
 */

class vsite_taxonomy_fbt extends os_boxes_default {
  
//  /**
//   * Implementation of boxes_content::options_defaults().
//   */
//  public function options_defaults() {
//    $options = parent::options_defaults();
//
//    $options['feed'] = 'all';
//    $options['number_of_items'] = 10;
//
//    return $options;
//  }
//
//  /**
//   * Implementation of boxes_content::options_form().
//   */
//  public function options_form(){
//
//    $form['feed'] = array(
//      '#type' => 'select',
//      '#title' => t('Feed'),
//      '#options' => array(
//      ),
//      '#required' => TRUE,
//    );
//
//    $form['number_of_items'] = array(
//      '#type' => 'textfield',
//      '#title' => t('Number of items to display'),
//      '#default_value' => $this->options['number_of_items']
//    );
//
//    $form += parent::options_form();
//    return $form;
//  }
//
//  /**
//   * Implementation of boxes_content::render().
//   */
//  public function render() {
//    //@todo
//    $view = views_get_view('scholar_reader');
//    $view->set_display('block_2');
//    $view->display_handler->set_option('items_per_page', $this->options['number_of_items']);
//
//    if ($this->options['feed'] != 'all') {
//      $view->set_arguments(array($this->options['feed']));
//    }
//
//    $block = parent::render();
//    $output = $view->execute_display();
//    $block['content'] = $output['content'];
//    return $block;
//  }

}