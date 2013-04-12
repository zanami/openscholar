<?php

function os_wysiwyg_link_insert() {
  module_invoke_all('suppress', TRUE);
  
  module_load_include('inc', 'file_entity', 'file_entity.pages');
  $tabs = array(
    'external' => array(
      'title' => 'Webpage',
    	'form' => '_os_link_external_form',
    ),
    'email' => array(
      'title' => 'E-mail',
      'form' => '_os_link_email_form',
    ),
    'file' => array(
      'title' => 'Files',
      'page' => '_os_link_get_view',
      'params' => array(),
    ),
    'page' => array(
      'title' => 'Page',
      'form' => '_os_link_internal_form',
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
    );
    if (isset($t['form'])) {
      $output['tabs'][$k]['form'] = drupal_get_form($t['form'], (!empty($t['params'])?$t['params']:array()));
    }
    elseif (isset($t['page'])) {
      $output['tabs'][$k]['page'] = $t['page']((!empty($t['params'])?$t['params']:array()));
    }
    if (!empty($t['default'])) {
      $output['tabs']['#default_tab'] = 'edit-'.$k;
      $output['tabs']['tabs__active_tab']['#value'] = 'edit-'.$k;
    }
    if (isset($output['tabs'][$k]['page']['upload']['upload']['#file']) && $output['tabs'][$k]['page']['upload']['upload']['#file']) {
      $entity = $output['tabs'][$k]['page']['upload']['upload']['#file'];
      $commands = array();
      if (isset($entity->fid)) {
        media_browser_build_media_item($entity);
        drupal_add_js(array('media' => array('files' => array($entity))), 'setting');
        
        $commands[] = ajax_command_prepend('#media-browser-library-list', '<li id="media-item-'.$entity->fid.'" class="media-item-new">'.$entity->preview.'</li>');
        $commands[] = array(
          'command' => 'switchTab',
          'tab' => 'edit-file'
        );
        $commands[] = array(
          'command' => 'clickOn',
          'target' => '#media-item-'.$entity->fid.' div.media-item',
        );
      }
      // return results to ajax script
      $output = array(
        '#type' => 'ajax',
        '#commands' => $commands,
      );
      ajax_deliver($output);
      exit;
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
  
  $form['coming_soon'] = array(
    '#markup' => t('Coming soon!'),
  );
  /*$form['internal'] = array(
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
  );*/
  
  return $form;
}

function _os_link_get_view($params) {
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
        'upload' => drupal_get_form('file_entity_add_upload', array(
          'file_directory' => os_files_get_save_path(),
          'no_redirect' => TRUE
        )),
      	'library' => $build,
        'actions' => array(
          '#type' => 'actions',
          '#attributes' => array(
            'class' => array('form-actions')
          ),
          'insert' => array(
            '#type' => 'submit',
            '#value' => t('Insert'),
          )
        ),
        'edit' => array(
          '#type' => 'container',
          '#id' => 'file-edit-section',
          '#attributes' => array(
            'id' => 'file-edit-section',
          ),
          'no_selection' => array(
            '#type' => 'container',
            '#id' => 'file-edit-no-selection',
            '#attributes' => array(
              'id' => 'file-edit-no-selection',
            )
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

