<?php

/**
 * @file
 * The class for "Edit/Display Node" type widgets.
 */

abstract class os_boxes_modalframe extends os_boxes_default {

  static $latest = null;

  public function __construct() {
    parent::__construct();
    self::$latest = $this;
  }

  final public function get_config_path() {
      return $this->config_path().'?modal&box='.($this->delta?$this->delta:$this->plugin_key);
  }

  protected static function config_path($box) {
    return '';
  }
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
    return array();
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {

    $block = parent::render();

    return $block;
  }

  /**
   *
   * Children should add values to $form_state['values']['box']
   * Then call the parent submit to create and save a box
   * @param unknown_type $form_state
   */
  static public function submit($form, $form_state) {
    $box = &$form_state['values']['box'];

    if (!isset($box['delta'])) {
      $box['delta'] = os_boxes_create_delta();
    }
    $form_state['values'] = $box;
    boxes_box_form_submit($form, $form_state);

    return self::$latest;
  }
}