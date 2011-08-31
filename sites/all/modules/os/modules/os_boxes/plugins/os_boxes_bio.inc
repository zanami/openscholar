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
    
    $form = parent::options_form();
    
    $form['nid']['#type'] = 'value';
    if (($nid = $this->options['nid']) == FALSE) {
      //bio node created after this post.  cache the nid.
      $vsite = vsite_get_vsite();
      if (($bio_node = content_profile_load('bio', $vsite->group->uid, $lang = '')) == FALSE) {
		$nid = 0;
      }
      $nid = $bio_node->nid;
    }
    $form['nid']['#value'] = $nid;
    
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
  	$block = parent::render();    
  	
  	//no nid option.  
    if (!$this->options['nid']) {
      //check if a bio has been set
      if (($vsite = vsite_get_vsite()) == FALSE) {
        return array();
      }
  
	  if (($bio_node = content_profile_load('bio', $vsite->group->uid, $lang = '')) == TRUE) {
        //we have a bio.  set it here and try again.
        $this->options['nid'] = $bio_node->nid;
        $block = parent::render();    
      } else {
        //new user - no bio yet.
	    $block['content'] = scholar_front_welcome();
      } 
    }

    return $block;
  }

}