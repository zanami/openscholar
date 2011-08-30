<?php

/**
 * @file
 * The class for "CC license" type widgets.  
 */

class os_boxes_bio extends os_boxes_node {
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){    

    //get nid of this vsite's bio
    if (($vsite = vsite_get_vsite()) == FALSE) {
      return array(); 
    }

    if (($bio_node = content_profile_load('bio', $vsite->group->uid, $lang = '')) == false) {
      return array();
    }
    
    
    $form = parent::options_form();
    
    $form['nid']['#type'] = 'value';
    $form['nid']['#value'] = $bio_node->nid;
    
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    if (!isset($this->options['nid'])) {
      //get a nid if newly created bio box hasn't been configured yet.  this shouldn't happen, and yet...
      if (($vsite = vsite_get_vsite()) == FALSE) {
        return array(); 
      }
  
      $bio_node = content_profile_load('bio', $vsite->group->uid, $lang = '');
      if(!$bio_node) {
        return array();
      }
      
      $this->options['nid'] = $bio_node->nid;
        
    }
    
    $block = parent::render();    
    /*

    //won't be blurb for long.  blurb text is to be copied to a simple box instance for each user with a blurb.
    $blurb = $bio_node->field_hook_text[0]['value'];
    $output = strlen($blurb) ? $blurb : node_teaser($bio_node->body, null,400) ;
    // trim the output's length
    $output .= l(" more", $bio_node->path,array(attributes=>array('class'=>'more')));
    $block['content'] = $output;
    */
    return $block;
  }

}