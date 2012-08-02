<?php

class os_boxes_site_logo extends os_boxes_default {

    function options_defaults() {
      $options = parent::options_defaults();

      $options += array(
        'fid' => 0,
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
        '#process' => array(
          '_os_boxes_site_logo_process',
        ),
      );

      if (module_exists('vsite') && $space = spaces_get_space()) {
        $form['file']['#upload_location'] = 'public://'.$space->group->purl.'/files';
      }

      $form['x'] = array(
        '#type' => 'hidden',
        '#default_value' => $this->options['x'],
      );

      $form['y'] = array(
        '#type' => 'hidden',
        '#default_value' => $this->options['y'],
      );

      $form['height'] = array(
        '#type' => 'hidden',
        '#default_value' => $this->options['height'],
      );

      $form['width'] = array(
        '#type' => 'hidden',
        '#default_value' => $this->options['width'],
      );

      return $form;
    }

    function options_submit($form, $form_state) {
      $form = parent::options_submit($form, $form_state);

      // do the cropping bit now
      dpm($form_state);


      if (module_exists('vsite') && $space = spaces_get_space()) {
        $node = $space->group;

        $node->field_site_logo[LANGUAGE_NONE][0] = (array)$file + array(
          'cropbox_x' => $this->options['x'],
          'cropbox_y' => $this->options['y'],
          'cropbox_height' => $this->options['height'],
          'cropbox_width' => $this->options['width'],
        );
      }
    }

    function render() {
      $block = parent::render();

      $file = file_load($this->options['fid']);
      $output = file_view($file, 'original');
      $block['content'] = $output;

      return $block;
    }
}

function _os_boxes_site_logo_process($element, $form_state, $form) {
  $element = file_managed_file_process($element, $form_state, $form);
  $fid = isset($element['#value']['fid']) ? $element['#value']['fid'] : 0;

  if ($fid && $element['#file']) {
    unset($element['filename']);
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