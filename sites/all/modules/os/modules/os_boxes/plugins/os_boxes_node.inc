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

    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){
    //@todo just for vsite for now but it can be used outside os    
    vsite_include('nodeautocomplete');
    $form['nid'] = vsite_node_autocomplete_element($title = NULL);
    $form += parent::options_form();
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {

    $block = parent::render();    
    
    if (($vsite = vsite_get_vsite()) == FALSE) {
      return array(); 
    }

    $nid = $this->options['nid'];
    //nid is saved as "title [nid:$NID]".  extract the number. 
    if (is_string($nid) && (int)$nid == 0) {
      $nid = (int)end(split(':', $nid));
    }
    $node = node_load($nid);
    
    $output = node_view($node, 'teaser', FALSE, FALSE);  //@todo : make this configurable
    $block['content'] = $output;
    
    return $block;
  }

}