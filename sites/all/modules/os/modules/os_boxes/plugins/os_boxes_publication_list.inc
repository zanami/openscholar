<?php

/**
 * @file
 * The class for "Listing Publications by Type" widget.
 */

class os_boxes_publication_list extends os_boxes_default {
  
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();

    $options['pub_types'] = array();
    $options['number_of_items'] = 6;

    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){
    
    $q = db_query("SELECT tid, name FROM {biblio_types} WHERE tid > 0 AND visible = 1");
    $options = array();
    while ($a = db_fetch_array($q)) $options[$a['tid']] = $a['name'];

    //Set to all types if none have been selected
  	if(!count($this->options['pub_types'])) $this->options['pub_types'] = array_keys($options);

  	$form['number_of_items'] = array(
      '#type' => 'textfield',
      '#title' => 'Number of displayed posts',
      '#default_value' => $this->options['number_of_items'],
      '#size' => 10
    );
    
    $form['pub_types'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => $this->options['pub_types'],
      '#title' => t('Show the following types'),
      '#attributes' => array('class' => 'pub_type'),
      '#description' => t('Select the publication types you want to appear in the widget.')
    );
    
    $form += parent::options_form();
    return $form;
  }
  
  /**
   * Custom submit hander to perform post_submit actions
   */
  public static function options_validate(&$form,&$form_state){
  	
    if(array_keys($form_state['values']['pub_types']) == array_keys($form['options']['pub_types']['#options'])){
    	$form_state['values']['pub_types'] = array();
    }//All selected is the same as no filter, set to none selected
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    //@todo
    $block = parent::render();

    $view_name = 'publication_types';
    $display_id = "page_1";
    
    if ($view = views_get_view($view_name)) {
      if ($view->access($display_id)) {
        $view->set_display($display_id);
        if (count($this->options['pub_types'])) {
          $filter = array(
            'operator' => 'in',
            'value' => $this->options['pub_types'],
            'group' => '0',
            'exposed' => FALSE,
            'expose' => array(
              'operator' => FALSE,
              'label' => '',
            ),
            'id' => 'tid',
            'table' => 'biblio_types',
            'field' => 'tid',
            'override' => array(
              'button' => 'Use default',
            ),
            'relationship' => 'none',
          );
          $view->set_item($display_id, 'filter', 'tid', $filter);
        }
        $output = $view->execute_display();
        $view->destroy();
        $block['content'] = $output['content'];
      }
      $view->destroy();
    }

    return $block;
  }

  /**
   * Return the cache parameters for this box
   */
  public function cache_info(){

//    //Clear when biblio nodes are saved
//    $cache_key = array(
//      'os_og' => array('current' => 'current'),
//      'node' => array('biblio' => 'biblio'),
//    );
//
//    return array('type' => BLOCK_CACHE_CONTENT_CACHE, 'keys' => $cache_key);

    return array(); //caching disabled so ajax pager gets init'ed
  }

}