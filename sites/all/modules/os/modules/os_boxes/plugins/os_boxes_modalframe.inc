<?php

/**
 * @file
 * The class for "Edit/Display Node" type widgets.
 */

abstract class os_boxes_modalframe extends os_boxes_default {
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    $options['path_edit'] = '';
    $options['path_display'] = '';

    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   *
   */
  public function options_form() {
    // this is a modalframe.
    // Redirect the iframe contents straight to the edit path
    if (isset($_GET['modal'])) {
      $send = $this->new?$this->plugin_key:$this->delta;
      drupal_goto($this->options['path_edit'], 'modal&box='.$send);
    }
    // we aren't a modalframe?
    // Then what are you doing in the modalframe box?
    $form = parent::options_form();
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {

    $block = parent::render();

    // just render the path
    $block['content'] = menu_execute_active_handler($this->options['path_display']);

    return $block;
  }

/*
 * Probably not needed anymore

  private function hide_form_elements_recursive(&$form, $hide = false) {
    if ($this->options['fields'] == '*') return;
    foreach (element_children($form) as $key) {

      if (!in_array($key, $this->options['fields']) || $hide) {
        $this->hide_form_elements_recursive($form[$key], true);

        if ($form[$key]['#type'] != 'hidden') {
          $form[$key]['#access'] = FALSE;
        }
        if (isset($form[$key]['#type'])) {
          $form[$key]['#type'] = 'value';
          if (isset($form[$key]['#default_value'])) {
            $form[$key]['#value'] = $form[$key]['#default_value'];
          }
        }
      }
      else {
        //$this->hide_form_elements_recursive($form[$key], $hide);
      }
    }
  }

  private function hide_view_elements_recursive(&$node) {
    if ($this->options['fields'] == '*') return;

  }
  */
}