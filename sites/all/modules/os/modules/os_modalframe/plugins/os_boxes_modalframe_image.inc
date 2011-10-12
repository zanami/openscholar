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
      $whitelist = array('title', 'field_gallery_image');
      foreach (element_children($form) as $child) {
        if ($form[$child]['#type'] != 'value' && $form[$child]['#type'] != 'hidden') {
          if (!in_array($child, $whitelist)) {
            $form[$child]['#type'] = 'value';
          }
        }
      }

      $form['buttons']['#type'] = 'markup';
      $form['buttons']['preview']['#access'] = false;
      $form['buttons']['delete']['#access'] = false;
      $form['buttons']['preview_changes']['#access'] = false;
    }
  }

  static public function submit($form, $form_state) {

    $form_state['values']['box']['nid'] = $form_state['nid'];
    return parent::submit($form, $form_state);
  }

  public function access() {
    if ($vsite = vsite_get_vsite()) {
      return $vsite->access_feature('edit', 'scholar_image_gallery');
    }
    else {
      return vsite_node_menu_access();

    }
  }
}