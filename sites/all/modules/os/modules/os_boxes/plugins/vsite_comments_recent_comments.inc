<?php

/**
 * @author rbran100
 * @category boxes
 *
 * The class for showing comments for a given content-type
 */

class vsite_comments_recent_comments extends os_boxes_default {

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();

    $options['number_of_items'] = 6;
    
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){

    $form['node_type'] = array(
      '#type' => 'select',
      '#title' => t('Comments From'),
      '#options' => array(
      ),
      '#required' => TRUE,
      '#default_value' => $this->options['node_type'],
      '#description' =>  "Choose the content type that you would like to display comments from."
    );
    
    // os/vsite integration - lists correct content types
    if (module_exists('vsite') && vsite_get_vsite()) {
      vsite_include('vsiteapi');
      //Get public and private types
      $node_options = vsite_content_types(array(1,2));
      drupal_alter('vsite_content_types', $node_options);
    }
    
    if (! isset($node_options)) {
      $node_options = node_get_types('names');
    }
    
    //Sort by node name
    asort($node_options);
    foreach ( $node_options as $type => $name ) {
      $form['node_type']['#options'][$type] = t('!type posts', array(
        '!type' => ucfirst($name),
      ));
    }
    
    $form['number_of_items'] = array(
      '#type' => 'select',
      '#title' => t('Number of comments to display'),
      '#options' => array(
                      1 => "1",
                      2 => "2",
                      3 => "3",
                      4 => "4",
                      5 => "5",
                      6 => "6",
                      7 => "7",
                      10 => "10",
                      15 => "15",
                      20 => "20",
                      25 => "25",
                    ),
      '#default_value' => $this->options['number_of_items']
    );

    $form += parent::options_form();
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $block = parent::render();
    
    switch ($this->node_comment_setting($this->options['node_type'])){
    	
    	
    }
    
    if(strlen($comments_html)) $block['content'] = $terms_html;

    return $block;
  }
  
  /**
   * Determine if this box is avalible for use
   */
  public function access(){
  	if($this->options['node_type'] && vsite_get_vsite()){
  		vsite_include('vsiteapi');
      $avalible_types = array_keys(vsite_content_types(array(2, 1)));
      
      if(!in_array($this->options['node_type'],$avalible_types)){
      	return false;
      }//Disabled Type
  	}
  	
  	//Check if commenting is enabled
  	return vsite_comments_admin_access($this->options['node_type']);
  }
  
  /**
   * Return the cache parameters for this box
   */
  public function cache_info(){
    
  	$cache_key = false;
  	switch($this->node_comment_setting($this->options['node_type'])){
  		case 'drupal':
  			//Adding comments triggers a node clear
		    $cache_key = array(
		      'os_og' => array('current' => 'current'),
		      'node' => array($this->options['node_type'] => $this->options['node_type']),
		      'comment' => array(
		        'changed' => 1,
		      )
		    );
  		break;
  	}
    
  	if(!$cache_key){
  		//Do not cache
  		return array();
  	}
  
    $cache_params = array('type' => BLOCK_CACHE_CONTENT_CACHE, 'keys' => $cache_key);

    return $cache_params;
  }
  
  protected function node_comment_setting($node_type){
  	//Get Feature/node comment settings
    $default_setting = variable_get("comment_{$node_type}",0)?'drupal':0;
    
    return variable_get("vsite_comments_comment_type_{$node_type}", $default_setting);
  }

}