<?php

class os_boxes_site_logo extends os_boxes_default {

    function options_defaults() {
      $options = parent::options_defaults();

      $options += array(
        'fid' => NULL,
        'x' => 0,
        'y' => 0,
        'height' => 100,
        'width' => 150,
      );

      return $options;
    }

    function options_form(&$form_state) {
      $form = parent::options_form($form_state);

      if (!module_exists('imagefield_crop')) {
        $form['warning'] = array(
          '#markup' => t('The imagefield_crop module must be enabled for this box to function.'),
        );
        return $form;
      }

      $form['file'] = array(
        '#type' => 'managed_file',
        '#title' => t('Site Logo'),
        '#description' => t('Upload a photo and position the cropping tool over it if necessary.'),
        '#required' => TRUE,
        '#process' => array(
          '_os_boxes_site_logo_process',
        ),
        '#default_value' => $this->options['fid'],
        '#dimensions' => array(
          'x' => $this->options['x'],
          'y' => $this->options['y'],
          'height' => $this->options['height'],
          'width' => $this->options['width'],
        ),
      );

      if (module_exists('vsite') && $space = spaces_get_space()) {
        $form['file']['#upload_location'] = 'public://'.$space->group->purl.'/files';
      }

      return $form;
    }

    function options_submit($form, $form_state) {
      // do the cropping bit now
      $file_data = $form_state['input']['file'];
      $file = file_load($file_data['fid']);

      $item = array(
        'cropbox_x' => $file_data['x'],
        'cropbox_y' => $file_data['y'],
        'cropbox_height' => $file_data['height'],
        'cropbox_width' => $file_data['width'],
      );

      $scale = array(
        'width' => 180,
        'height' => 140,
      );

      // this creates two images
      // $orig is the original, unmodified image
      // its saved so we can refer to it later
      // $file is original uploaded image that is then cropped to fit
      // it is the file that is displayed in render
      // it 'contains' the orig file, through the file usage table
      $orig = _imagefield_crop_file_to_crop($file->fid);
      if ($orig->fid == $file->fid) {
        $orig = imagefield_crop_create_copy($file);
        file_usage_add($orig, 'imagefield_crop', 'file', $file->fid);
      }
      _imagefield_crop_resize(drupal_realpath($orig->uri), $item, $scale, $file);
      file_save($file);

      $this->options['fid'] = $file->fid;

      $this->options['x'] = $file_data['x'];
      $this->options['y'] = $file_data['y'];
      $this->options['height'] = $file_data['height'];
      $this->options['width'] = $file_data['width'];

      if (module_exists('vsite') && $space = spaces_get_space()) {
        $node = $space->group;

        $node->field_site_logo[LANGUAGE_NONE][0] = (array)$file + array(
          'cropbox_x' => $file_data['x'],
          'cropbox_y' => $file_data['y'],
          'cropbox_width' => $file_data['width'],
          'cropbox_height' => $file_data['height'],
        );

        node_save($node);
      }
    }

    function render() {
      $block = parent::render();

      try {
        $file = file_load($this->options['fid']);
        $output = file_view($file, 'media_original');
        $block['content'] = render($output);
      }
      catch (Exception $e) {
        return array();
      }

      return $block;
    }
}

function _os_boxes_site_logo_process($element, $form_state, $form) {
  $element = file_managed_file_process($element, $form_state, $form);
  $element['remove_button']['#submit'][] = '_os_boxes_site_logo_remove_handler';

  $fid = isset($element['#value']['fid']) ? $element['#value']['fid'] : 0;

  if ($fid && $element['#file']) {
    unset($element['filename']);
    // file_to_crop() take the cropped image and gets the original back
    $file_to_crop = _imagefield_crop_file_to_crop($element['#file']->fid);
    $element['preview'] = array(
      '#type' => 'markup',
      '#file' => $file_to_crop, // This is used by the #process function
      '#prefix' => '<div class="imagefield-crop-preview">',
      '#suffix' => '</div>',
      '#process' => array('_os_boxes_site_logo_preview_process'),
      '#theme' => 'imagefield_crop_preview',
      '#markup' => theme('image', array(
        'path' => $element['#file']->uri,
        'getsize' => FALSE,
        'attributes' => array('class' => 'preview-existing'))
      ),
    );
    $element['cropbox'] = array(
      '#markup' => theme('image', array(
        'path' => $file_to_crop->uri,
        'attributes' => array(
          'class' => 'cropbox',
          'id' => $element['#id'] . '-cropbox')
        )
      ),
    );
    $settings = array(
      $element['#id'] => array(
        'box' => array(
          'ratio' => 180/140,
          'box_width' => 400,
          'box_height' => 400,
        ),
        'minimum' => array(
          'width' => NULL,
          'height' => NULL,
        ),
      ),
    );
    $element['#attached']['js'][] = array(
      'data' => array('imagefield_crop' => $settings),
      'type' => 'setting',
      'scope' => 'header',
    );
    $element['#attached']['js'][] = drupal_get_path('module', 'imagefield_crop').'/Jcrop/js/jquery.Jcrop.js';
    $element['#attached']['js'][] = drupal_get_path('module', 'imagefield_crop').'/imagefield_crop.js';
    $element['#attached']['css'][] = drupal_get_path('module', 'imagefield_crop').'/Jcrop/css/jquery.Jcrop.css';

    $element['x'] = array(
      '#type' => 'hidden',
      '#default_value' => $element['#dimensions']['x'],
      '#attributes' => array(
        'class' => array(
          'edit-image-crop-x'
        )
      )
    );

    $element['y'] = array(
      '#type' => 'hidden',
      '#default_value' => $element['#dimensions']['y'],
      '#attributes' => array(
        'class' => array(
          'edit-image-crop-y'
        )
      )
    );

    $element['height'] = array(
      '#type' => 'hidden',
      '#default_value' => $element['#dimensions']['height'],
      '#attributes' => array(
        'class' => array(
          'edit-image-crop-height'
        )
      )
    );

    $element['width'] = array(
      '#type' => 'hidden',
      '#default_value' => $element['#dimensions']['width'],
      '#attributes' => array(
        'class' => array(
          'edit-image-crop-width'
        )
      )
    );
  }

  return $element;
}

function _os_boxes_site_logo_preview_process($element, $form_state, $form) {
  $element = @imagefield_crop_widget_preview_process($element, $form_state, $form);

  $element['#attached']['js'][0]['data']['imagefield_crop']['edit-file']['preview']['width'] = 180;
  $element['#attached']['js'][0]['data']['imagefield_crop']['edit-file']['preview']['height'] = 140;

  $element['#imagefield_crop']['#width'] = 180;
  $element['#imagefield_crop']['#height'] = 140;

  return $element;
}

function _os_boxes_site_logo_remove_handler($form, $form_state) {
  $parents = $form_state['triggering_element']['#array_parents'];
  $element = drupal_array_get_nested_value($form, $parents);

  if (isset($element['#file'])) {
    $orig = _imagefield_crop_file_to_crop($element['#file']->fid);
    file_usage_delete($orig, 'imagecrop');
    file_delete($orig);
    file_delete($element['#file']);
    unset($element['#file']);
  }
}