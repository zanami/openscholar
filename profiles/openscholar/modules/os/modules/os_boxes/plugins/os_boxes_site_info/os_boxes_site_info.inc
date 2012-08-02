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

  function options_form(&$form_state) {
    $form = parent::options_form($form_state);

    $form['site_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => $this->options['site_title'],
      '#size' => 60,
    );

    $form['descrip'] = array(
      '#type' => 'textfield',
      '#title' => t('Description'),
      '#default_value' => $this->options['descrip'],
      '#description' => t('A brief description of your web site.'),
      '#size' => 60,
    );

    $form['address1'] = array(
      '#type' => 'textfield',
      '#title' => t('Address Line 1'),
      '#default_value' => $this->options['address1'],
      '#size' => 30,
      '#description' => t('Ex. 31 Harvard Ave.'),
    );

    $form['address2'] = array(
      '#type' => 'textfield',
      '#title' => t('Address Line 2'),
      '#default_value' => $this->options['address2'],
      '#size' => 30,
      '#description' => t('Ex. Cambridge, MA 02138'),
    );

    return $form;
  }

  function options_submit($form, $form_state) {
    if (module_exists('vsite') && $space = spaces_get_space()) {
      // populate the vsite with this information
      $node = $space->group;
      $node->title = $this->options['site_title'];

      $node->field_site_description[LANGUAGE_NONE][0] = array(
        'value' => $this->options['descrip'],
        'format' => NULL,
      );
      $node->field_site_address[LANGUAGE_NONE][0] = array(
        'value' => $this->options['address1'].'<br>'.$this->options['address2'],
        'format' => NULL,
      );
      node_save($node);
    }
  }

  function render() {
    $block = parent::render();
    $output = '';

    $output = '<h1>'.$this->options['site_title'].'</h1>
    <h2>'.$this->options['descrip'].'</h2>
    <p>'.$this->options['address1'].'<br>'.$this->options['address2'].'
    '.l('(email)', 'contact_owner').'</p>';

    $block['content'] = $output;
    return $block;
  }
}