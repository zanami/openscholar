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
    $options['more_link'] = 0;  // because use_pager defaults to 1
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){
    
    $form = spaces_simpleviews_get_edit_form($this->options);
    $form['more_link'] = array(
      '#type' => 'checkbox', 
      '#title' => t('Create "more" link'),
      '#description' => t('This will add a more link to the bottom of this widget, which will link to the page listing all the posts of type selected above'),    
    
    );
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

    // @todo handle "more" link
    
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


