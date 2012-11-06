<?php

class os_boxes_site_info extends os_boxes_default {

  function options_defaults() {
    $options = parent::options_defaults();

    $options += array(
      'site_title' => 'Your Name',
      'descrip' => 'Hover over here, and then click the gear to change me.',
      'address1' => '13 Somewhere Dr.',
      'address2' => 'That Town, USA',
    );

    return $options;
  }

  function box_form(&$form_state) {
    $form = parent::options_form($form_state);

    $form['site_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => $this->options['site_title'],
      '#size' => 60,
    );

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
        if ($v == 'group_site_info') {
          $form[$c] = $temp[$c];
        }
      }
      /*
      $addr = field_get_items('node', $group, 'field_site_address');
      if (isset($addr[0])) {
        $addr = explode('<br>', $addr[0]['value']);
      }
      else {
        $addr = array('', '');
      }
      $descrip = field_get_items('node', $group, 'field_site_description');
      $this->options = array_merge($this->options, array(
        'site_title' => $group->title,
        'descrip' => isset($descrip[0])?$descrip[0]['value']:'',
        'address1' => $addr[0],
        'address2' => $addr[1],
      ));
      */
    }
//
//    $form['descrip'] = array(
//      '#type' => 'textfield',
//      '#title' => t('Description'),
//      '#default_value' => $this->options['descrip'],
//      '#description' => t('A brief description of your web site.'),
//      '#size' => 60,
//    );
//
//    $form['address1'] = array(
//      '#type' => 'textfield',
//      '#title' => t('Address Line 1'),
//      '#default_value' => $this->options['address1'],
//      '#size' => 30,
//      '#description' => t('Ex. 31 Harvard Ave.'),
//    );
//
//    $form['address2'] = array(
//      '#type' => 'textfield',
//      '#title' => t('Address Line 2'),
//      '#default_value' => $this->options['address2'],
//      '#size' => 30,
//      '#description' => t('Ex. Cambridge, MA 02138'),
//    );

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
        'title' => array(
          '#prefix' => '<h1>',
          '#suffix' => '</h1>',
          '#markup' => check_plain($group->title),
          '#weight' => 0,
        ),
        'contact' => array(
          '#markup' => l('(email)', 'contact_owner'),
          '#weight' => 100,
        )
      );
      field_attach_prepare_view('node', array($group->nid => $group), 'full');
      $temp = field_attach_view('node', $group, 'full');

      $children = element_children($temp);
      $props = array_diff(array_keys($temp), $children);

      foreach ($props as $p) {
        $content[$p] = $temp[$p];
      }

      foreach ($content['#group_children'] as $c => $v) {
        if ($v == 'group_site_info') {
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
      if ($g == 'group_site_info') {
        $node->{$c} = $form_state['values'][$c];
      }
    }
    node_save($node);
  }
  else {
    boxes_box_form_submit($form, $form_state);
  }
}