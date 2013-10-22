<?php

function os_files_settings_form() {
  $form = array();

  $value = '';
  $mappings = variable_get('os_files_extension_map', array());
  foreach ($mappings as $ext => $type) {
    $value .= "$ext: $type\r\n";
  }
  $form['os_files_extension_mappings'] = array(
    '#type' => 'textarea',
    '#default_value' => $value
  );

  $form = system_settings_form($form);
  $form['#submit'][] ='os_files_settings_form_submit';

  return $form;
}

function os_files_settings_form_submit($form, $form_state) {
  $mappings = array();
  $lines = explode("\n", $form_state['values']['os_files_extension_mappings']);
  foreach ($lines as $l) {
    $data = explode(': ', $l);
    $mappings[$data[0]] = $mappings[$data[1]];
  }

  variable_set('os_files_extension_map', $mappings);
}