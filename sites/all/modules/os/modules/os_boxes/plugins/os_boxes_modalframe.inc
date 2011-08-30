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
      return $this->config_path().'/modal/box/'.($this->delta?$this->delta:$this->plugin_key);
  }

  protected function config_path($box) {
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
    $old_title = drupal_get_title();

    // just render the path
    $block['content'] = menu_execute_active_handler($this->options['path_display']);
    drupal_set_title($old_title);

    return $block;
  }

  /**
   * Form Alter method
   * Children should treat this like a regular form alter.
   * Just run the parent at the end.
   */
  public function form_alter(&$form, &$form_state) {
    $box_form = array(
      '#tree' => true,
      'plugin_key' => array(
        '#value' => $this->plugin_key,
        '#type' => 'value',
      ),
      'delta' => array(
        '#value' => $this->delta,
        '#type' => 'value',
      ),
      'description' => array(
        '#value' => $this->description,
        '#type' => 'value',
      ),
      'title' => array(
        '#value' => $this->title,
        '#type' => 'value',
      ),
    );

    $form['box'] = $box_form;
    if (isset($form['buttons'])) {
      $form['buttons']['submit']['#submit'][] = 'os_boxes_modalframe_form_submit';
    }
    else {
      $form['#submit'][] = 'os_boxes_modalframe_form_submit';
    }
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