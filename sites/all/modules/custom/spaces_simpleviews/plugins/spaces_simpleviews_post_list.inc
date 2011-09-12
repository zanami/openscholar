<?php

/**
 * @file
 * The class for "list" type widgets.  It allows users to define widgets that are
 * list of posts (i.e. latest anouncements, blogs etc).
 * It's simply an interface to an existing view (as in views module)
 */

class spaces_simpleviews_post_list extends boxes_box {
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = spaces_simpleviews_default_data();
    $options['number_of_items'] = 6; //@todo whats a good default ?
    
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){
    
    $form = spaces_simpleviews_get_edit_form($this->options);
    
    //$form += parent::options_form();
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    
    //prepare the view
    $simpleview = $this->options;
    $view = spaces_simpleviews_build_view($simpleview);
    $view->init();
    $view->set_display('default');

    //number of items
    if (isset($simpleview['number_of_items']) && (int)$simpleview['number_of_items'] > 0) {
      $view->display_handler->set_option('items_per_page', (int)$simpleview['number_of_items']);
    }

    $title = isset($this->title) ? check_plain($this->title) : NULL;
    $block = array(
      'title' => $title,
      'subject' => $title,
      'delta' => $this->delta,
      'content' => $view->execute_display(),
    );
    
    return $block;
  }

}


