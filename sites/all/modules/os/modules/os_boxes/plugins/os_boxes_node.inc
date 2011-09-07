<?php

/**
 * @file
 * The class for "Node" type widgets.
 */

class os_boxes_node extends os_boxes_default {
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    $options['nid'] = 0;
    $options['teaser'] = 'teaser';

    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){
    //@todo just for vsite for now but it can be used outside os
    vsite_include('nodeautocomplete');
    $form['nid'] = vsite_node_autocomplete_element($title = NULL);
    $form['nid']['#default_value'] = $this->options['nid'];
    $form += parent::options_form();
    
    $form['teaser'] = array(
      '#type' =>  'radios',
      '#title' => 'Display Mode',
      '#default_value' => $this->options['teaser'],
      '#options' => array('teaser'=>'Teaser Only', 'full'=>'Full Node'),
      '#description' => 'Display entire node in this box or just the teaser.',
    );
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {

    $block = parent::render();
    
    if (($vsite = vsite_get_vsite()) == FALSE) {
      return $block;
    }

    $nid = $this->options['nid'];
    //nid is saved as "title [nid:$NID]".  extract the number.
    vsite_include('nodeautocomplete');
    if (is_string($nid) && (int)$nid == 0) {
      $nid = _vsite_node_autocomplete_title_to_nid($nid);
    }
    
    if((int)$nid != 0){
      $node = node_load($nid);
    }//valid nid?
    
    if($node){
      $teaser = ($this->options['teaser'] == 'teaser') ? TRUE : FALSE;
      $output = node_view($node, $teaser, FALSE, FALSE);  //@todo : make this configurable
      $block['content'] = $output;
    }
    
    return $block;
  }

}
