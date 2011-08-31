<?php

class os_boxes_modalframe_image extends os_boxes_modalframe {

  protected function config_path() {
    if ($this->options['nid']) {
      return 'node/'.$this->options['nid'].'/edit';
    }
    else {
      return 'node/add/image';
    }
  }

  public function options_defaults() {
    $options = parent::options_defaults();
    $options['nid'] = 0;
    return $options;
  }

  public function render() {
    $node = node_load($this->options['nid']);

    $block = parent::render();
    $block['content'] = content_format('field_gallery_image', $node->field_gallery_image[0], 'gallery_thumb_linked', $node);
    return $block;
  }

  public function form_alter(&$form, &$form_state) {
    if ($form['#id'] = 'node-form' && $form['form_id']['#value'] = 'image_node_form') {
      parent::form_alter($form, $form_state);

      // hide unnecessary fields
      $form['field_gallery']['#type'] = 'value';
      $form['options']['#type'] = 'value';
      $form['author']['#type'] = 'value';
      $form['book']['#type'] = 'value';
      $form['menu']['#type'] = 'value';
      $form['comment_settings']['#type'] = 'value';
      $form['path']['#type'] = 'value';
    }
  }
}