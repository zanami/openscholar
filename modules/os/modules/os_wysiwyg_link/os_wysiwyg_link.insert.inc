<?php

function os_wysiwyg_link_insert() {
  module_load_include('inc', 'file_entity', 'file_entity.pages');
  $tabs = array(
    'file' => array(
      'title' => 'Library',
      'form' => '_os_link_get_view',
      'params' => array(),
    ),
    'upload' => array(
      'title' => 'Upload',
      'form' => 'file_entity_add_upload',
      'params' => array(),
    ),
    'external' => array(
      'title' => 'Website',
    	'form' => '_os_link_external_form',
    ),
    'page' => array(
      'title' => 'Page',
      'form' => '_os_link_internal_form',
    ),
    'email' => array(
      'title' => 'E-mail',
      'form' => '_os_link_email_form',
    ),
  );
  
  if (isset($_GET['active']) && isset($tabs[$_GET['active']])) {
     $tabs[$_GET['active']]['default'] = true;
  }
  
  $output = array(
    'text' => array(
      '#type' => 'textfield',
      '#title' => t('Link Text'),
      '#description' => t('The text of the link. Leave blank to use the url of the link itself.'),
      '#name' => 'link-text',
    ),
    'tabs' => array(
      '#type' => 'vertical_tabs',
    )
  );
  foreach ($tabs as $k => $t) {
    $output['tabs'][$k] = array(
      '#type' => 'fieldset',
      '#title' => $t['title'],
      'form' => drupal_get_form($t['form'], (!empty($t['params'])?$t['params']:array()))
    );
    if (!empty($t['default'])) {
      $output['tabs']['#default_tab'] = 'edit-'.$k;
    }
  }
  
  $output['actions'] = array(
    '#type' => 'container',
    'insert' => array(
      '#type' => 'submit',
      '#value' => 'Insert',
    ),
    'cancel' => array(
      '#type' => 'submit',
      '#value' => 'Cancel',
    ),
    '#attributes' => array(
      'class' => array('insert-buttons'),
    ),
  );
  $path = drupal_get_path('module', 'os_wysiwyg_link');
  $output['#attached'] = array(
    'css' => array($path.'/theme/os_wysiwyg_link_form.css'),
    'js' => array($path.'/js/os_wysiwyg_link_form.js'),
  );
  
  return $output;
}

function _os_link_external_form($form_state) {
  $form = array();
  
  $form['external'] = array(
    '#type' => 'textfield',
    '#title' => t('Website URL'),
    '#description' => t('The URL of the external website.'),
  );
  
  $form['actions'] = array(
    '#type' => 'actions',
    'insert' => array(
      '#type' => 'submit',
      '#value' => 'Insert',
    )
  );

  return $form;
}

function _os_link_internal_form($form_state) {
  $form = array();
  
  $form['internal'] = array(
    '#type' => 'textfield',
    '#title' => t('Page'),
    '#description' => t('The page of your site to link to.'),
  );
  
  $form['actions'] = array(
    '#type' => 'actions',
    'insert' => array(
      '#type' => 'submit',
      '#value' => 'Insert',
    ),
  );
  
  return $form;
}

function _os_link_get_view() {
  $name = 'media_default';
  $display = 'media_browser_1';
  
  if ($view = views_get_view($name)) {
    if ($view->set_display($display)) {
      $build['#markup'] = $view->preview();

      // Allow the View title to override the plugin title.
      if ($title = $view->get_title()) {
        $build['#title'] = $title;
      }

      return array('library' => $build);
    }
  }
  
  return array();
}

function _os_link_email_form($form_state) {
  $form = array();
  
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail'),
  );
  
  return $form;
}

