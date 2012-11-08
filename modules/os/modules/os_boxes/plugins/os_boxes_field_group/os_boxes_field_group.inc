<?php

class os_boxes_field_group extends os_boxes_default {

  function options_defaults() {
    $options = parent::options_defaults();

    $options += array(
      'field_group' => '',
    );

    return $options;
  }

  function box_form(&$form_state) {
    $form = parent::options_form($form_state);

    if (module_exists('vsite') && $vsite = spaces_get_space()) {
      $group = $vsite->group;
      $temp = array();
      field_attach_form('node', $group, $temp, $form_state);
      $children = element_children($temp);
      $props = array_diff(array_keys($temp), $children);

      foreach ($props as $p) {
        $form[$p] = $temp[$p];
      }

      foreach ($form['#group_children'] as $c => $v) {
        if ($v == $this->options['field_group']) {
          $form[$c] = $temp[$c];
        }
      }
    }

    $form['actions'] = array(
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#submit' => array('os_boxes_site_info_submit'),
      ),
      'cancel' => array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#limit_validation_errors' => array(),
      ),
      '#weight' => 100
    );

    return $form;
  }

  function render() {
    $block = parent::render();

    $output = '';
    if (module_exists('vsite') && $vsite = spaces_get_space()) {
      $group = $vsite->group;
      $content = array(
        '#view_mode' => 'full',
      );
      field_attach_prepare_view('node', array($group->nid => $group), 'full');
      $temp = field_attach_view('node', $group, 'full');
      dpm($temp);

      $children = element_children($temp);
      $props = array_diff(array_keys($temp), $children);

      foreach ($props as $p) {
        $content[$p] = $temp[$p];
      }

      foreach ($content['#group_children'] as $c => $v) {
        if ($v == $this->options['field_group'] && isset($temp[$c])) {
          $content[$c] = $temp[$c];
        }
      }

      $output = render($content);
    }
    else {
      $output = '<h1>'.l($this->options['site_title'], '<front>').'</h1>
      <h2>'.check_plain($this->options['descrip']).'</h2>
      <p>'.check_plain($this->options['address1']).'<br>'.check_plain($this->options['address2']).'
      '.l('(email)', 'contact_owner').'</p>';
    }

    $block['content'] = $output;
    return $block;
  }
}

function os_boxes_site_info_submit($form, &$form_state) {
  $box = $form_state['box'];
  if (module_exists('vsite') && $space = spaces_get_space()) {
    // populate the vsite with this information
    $node = $space->group;
    $node->title = $form_state['values']['site_title'];

    foreach ($form['#group_children'] as $c => $g) {
      if ($g == $box->options['field_group']) {
        $node->{$c} = $form_state['values'][$c];
      }
    }
    node_save($node);
  }
  else {
    boxes_box_form_submit($form, $form_state);
  }
}