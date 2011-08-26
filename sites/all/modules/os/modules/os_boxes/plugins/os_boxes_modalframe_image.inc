<?php

class os_boxes_modalframe_image extends os_boxes_modalframe {

  public function options_defaults() {
    $options = parent::options_defaults();
    if ($this->options['nid'] == 0) {
     $options['path_edit'] = 'node/add/image';
    }
    else {
     $options['path_edit'] = 'node/'.$this->options['nid'].'/edit';
     $options['path_display'] = 'node/'.$this->options['nid'];
    }

    $options['nid'] = 0;
    return $options;
  }
}