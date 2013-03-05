<?php

function os_wysiwyg_link_insert() {
  module_invoke_all('suppress', TRUE);
  
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
     $tabs[$_GET['active']]['params'] = array('url' => $_GET['url']);
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
      'tabs__active_tab' => array(
        '#type' => 'hidden',
        '#default_value' => '',
        '#attributes' => array('class' => array('vertical-tabs-active-tab')),
      )
    )
  );
  foreach ($tabs as $k => $t) {
    $output['tabs'][$k] = array(
      '#type' => 'fieldset',
      '#title' => $t['title'],
      '#id' => 'edit-'.$k,
      'form' => drupal_get_form($t['form'], (!empty($t['params'])?$t['params']:array()))
    );
    if (!empty($t['default'])) {
      $output['tabs']['#default_tab'] = 'edit-'.$k;
      $output['tabs']['tabs__active_tab']['#value'] = 'edit-'.$k;
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
    'library' => array(
      array('media', 'media_browser_page'),
    ),
    'css' => array($path.'/theme/os_wysiwyg_link_form.css'),
    'js' => array($path.'/js/os_wysiwyg_link_form.js'),
  );
  
  return $output;
}

function _os_link_external_form($form, $form_state, $params) {
  $form = array();
  $args = func_get_args();
  
  $form['external'] = array(
    '#type' => 'textfield',
    '#title' => t('Website URL'),
    '#description' => t('The URL of the external website.'),
    '#default_value' => isset($params['url'])?$params['url']:'',
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

function _os_link_internal_form($form, $form_state, $params) {
  $form = array();
  
  $form['internal'] = array(
    '#type' => 'textfield',
    '#title' => t('Page'),
    '#description' => t('The page of your site to link to.'),
    '#default_value' => isset($params['url'])?$params['url']:'',
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

function _os_link_get_view($form, $form_state, $params) {
  $name = 'media_default';
  $display = 'media_browser_1';
  if (isset($params['url'])) {
    $_GET['fid'] = $params['url'];
  }
  
  if ($view = views_get_view($name)) {
    if ($view->set_display($display)) {
      $build['#markup'] = $view->preview();

      // Allow the View title to override the plugin title.
      if ($title = $view->get_title()) {
        $build['#title'] = $title;
      }
      
      $output = array(
      	'library' => $build,
        'actions' => array(
          '#type' => 'actions',
          'insert' => array(
            '#type' => 'submit',
            '#value' => t('Insert'),
          )
        )
      );

      return $output;
    }
  }
  
  return array();
}

function _os_link_email_form($form, $form_state, $params) {
  $form = array();
  
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail'),
    '#default_value' => isset($params['url'])?$params['url']:'',
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

