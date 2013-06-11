<?php

function os_files_upload_page($ajax = 'nojs') {
  $form_state = array();

  $form = drupal_build_form('os_files_upload', $form_state);
  if (empty($form_state['commands']) || $ajax == 'nojs') {
    return $form;
  }
  else {
    print ajax_render($form_state['commands']);
    exit;
  }
}

/**
 * Save files to a temporary location so we can check if a file with that URI already exists
 */
function os_files_upload($form, $form_state) {
  $form = array(
    '#action' => url('os/files/upload/ajax')
  );

  module_load_include('inc', 'media', 'includes/media.browser');
  $options = media_get_browser_params();
  form_load_include($form_state, 'inc', 'file_entity', 'file_entity.pages');

  $form['upload'] = array(
    '#type' => 'managed_file',
    '#title' => t('Upload a new file'),
    '#upload_location' => file_entity_upload_destination_uri($options).'/temporary',
    '#upload_validators' => file_entity_get_upload_validators($options),
    '#progress_indicator' => 'bar',
    '#required' => TRUE,
    '#pre_render' => array('file_managed_file_pre_render', 'file_entity_upload_validators_pre_render'),
  );
  dpm($form['upload']['#upload_location']);

  $form['actions'] = array(
    '#type' => 'actions',
    'save' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'os_files').'/theme/os_files_upload_form.js',
      ),
      'css' => array(
        drupal_get_path('module', 'os_files').'/theme/os_files_upload_form.css'
      )
    )
  );

  return $form;
}

function os_files_upload_submit($form, &$form_state) {
  $file = file_load($form_state['values']['upload']);

  $uri = str_replace('/temporary', '', $file->uri);
  $existing = file_uri_to_object($uri);
  if (isset($existing->fid)) {
    // a file with this uri already exist. Prompt to replace
    $form = drupal_get_form('os_files_upload_replace', $file);
    $form_state['commands'][] = ajax_command_html('#file-edit-section', drupal_render($form));
    $form_state['no_redirect'] = true;
  }
}

function os_files_replace_page($file) {
  $form_state = array(
    'build_info' => array(
      'args' => array(
        $file
      )
    )
  );

  $form = drupal_build_form('os_files_upload_replace', $form_state);
  if (empty($form_state['commands'])) {
    return $form;
  }
  else {
    print ajax_render($form_state['commands']);
    exit;
  }

}

function os_files_upload_replace($form, $form_state) {
  $file = $form_state['build_info']['args'][0];
  $form = array(
    '#action' => url('os/files/replace/'.$file->fid)
  );

  $uri = _os_files_perm_url($file->uri);
  $existing = file_uri_to_object($uri);

  $form['warning'] = array(
    '#markup' => t('A file with the name @name already exists at @uri. Would you like to replace it?', array('@name' => $existing->filename, '@uri' => $existing->origname))
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'yes' => array(
      '#type' => 'submit',
      '#value' => t('Yes, replace it.'),
    ),
    'no' => array(
      '#type' => 'submit',
      '#value' => t('No, rename it.')
    ),
    'cancel' => array(
      '#type' => 'submit',
      '#value' => t('I don\'t want to upload this after all.'),
    )
  );

  foreach (element_children($form['actions']) as $k) {
    $form['actions'][$k]['#attributes']['class'][] = 'use-ajax-submit';
  }

  return $form;
}

function os_files_upload_replace_submit($form, &$form_state) {
  $file = $form_state['build_info']['args'][0];
  switch ($form_state['values']['op']) {
    case t('Yes, replace it.'):
      $existing = file_uri_to_object(_os_files_perm_url($file->uri));
      _os_files_replace_from_temporary($file);
      $form_state['commands'][] = ajax_command_invoke('#media-item-'.$existing->fid, 'click');
      $form_state['commands'][] = ajax_command_invoke('.media-item-new', 'removeClass', array('media-item-new'));
      $form_state['commands'][] = ajax_command_invoke('#media-item-'.$existing->fid, 'addClass', array('media-item-new'));
      $form_state['commands'][] = ajax_command_invoke('#file-edit-section', 'empty');
      break;
    case t('No, rename it.'):
      file_move($file, _os_files_perm_url($file->uri));
      $form_state['commands'][] = ajax_command_invoke('#media-item-'.$file->fid, 'click');
      $form_state['commands'][] = ajax_command_invoke('.media-item-new', 'removeClass', array('media-item-new'));
      $form_state['commands'][] = ajax_command_invoke('#media-item-'.$file->fid, 'addClass', array('media-item-new'));
      $form_state['commands'][] = ajax_command_invoke('#file-edit-section', 'empty');
      break;
    case t('I don\'t want to upload this after all.'):
      file_delete($file);
      break;
  }
  $form_state['commands'][] = ajax_command_invoke('#edit-upload-upload', 'val', array());

  $form_state['no_redirect'] = true;
}

function _os_files_perm_url($uri) {
  return str_replace('temporary/', '', $uri);
}

function _os_files_replace_from_temporary($file) {
  file_unmanaged_copy($file->uri, _os_files_perm_url($file->uri), FILE_EXISTS_REPLACE);
  file_delete($file);
}
