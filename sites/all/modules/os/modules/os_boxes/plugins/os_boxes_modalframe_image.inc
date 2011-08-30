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
}