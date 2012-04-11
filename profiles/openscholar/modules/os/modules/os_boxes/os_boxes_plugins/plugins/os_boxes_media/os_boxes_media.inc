<?php

class os_boxes_media extends os_boxes_default {

  public function options_default() {
    return array (
      'fid' => 0,
    );
  }

  public function box_form(&$form_state) {
    module_load_include('inc', 'media', 'includes/media.browser');

    $box_fields = array(
      'title' => array(
        '#type' => 'hidden',
        '#default_value' => $this->title,
      ),
      'description' => array(
        '#type' => 'hidden',
        '#default_value' => $this->description,
      ),
      'plugin_key' => array(
        '#type' => '#value',
        '#value' => $this->plugin_key,
      ),
      'delta' => array(
        '#type' => '#value',
        '#value' => $this->delta,
      ),
      'fid' => array(
        '#type' => 'hidden',
        '#default_value' => 16, //$this->options['fid'],
      ),
    );
    $form = media_browser();
    $plugins = &$form['tabset']['plugins'];
    foreach ($plugins as &$p) {
      if (isset($p['form'])) {
        $p['form'] += $box_fields;
        $p['form']['#submit'][] = array($this, 'options_submit');
      }
    }

    // copied from boxes.module
    $form['description'] = array(
      '#type' => 'textfield',
      '#title' => t('Widget Description'),
      '#default_value' => $this->description,
      '#maxlength' => 64,
      '#description' => t('A brief description of your widget. This is only seen by use.'),
      '#required' => TRUE,
      '#weight' => -19,
    );
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Widget Title'),
      '#maxlength' => 64,
      '#description' => t('The title of the widget as shown to the user.'),
      '#default_value' => $this->title,
      '#weight' => -18,
    );

    $form['#attached']['js'][] = drupal_get_path('module', 'os_boxes_plugins').'/plugins/os_boxes_media/os_boxes_media_form.js';

    $form['actions'] = array(
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Save'),
      ),
      'cancel' => array(
        '#type' => 'link',
        '#title' => t('Cancel'),
        '#href' => '',
        '#attributes' => array('class' => array('button', 'button-no', 'fake-cancel')),
        '#weight' => 100,
      ),
    );
    dpm($form);
    return $form;
  }

  public function options_submit($form, &$form_state) {
    // handle the file upload
    // or the selection
  }

  public function render() {
    $block = parent::render();

    $block['content'] = drupal_render(file_view($this->options['fid']));
    return $block;
  }
}