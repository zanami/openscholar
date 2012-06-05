<?php

function cp_layout_full_form($form, $form_state, $ctx) {
  $form = array();
  ctools_include('ajax');
  ctools_include('modal');
  ctools_include('layout', 'os');

  // Temporary: To be replaced by API function later
  $contexts = context_context_list();
  $ctx = $ctx?$ctx:'os_front';

  $form['context_selection'] = array(
    '#type' => 'select',
    '#title' => t('Site Section'),
    '#options' => $contexts,
    '#default_value' => $ctx,
    '#multiple' => false,
  );

  $use_content_regions = false;
  if ($ctx == 'os_front') {
    $use_content_regions = true;
  }
  $blocks = os_layout_get_multiple(array('os_public', $ctx), true, true);

  $form['context'] = array(
    '#type' => 'value',
    '#value' => $ctx,
  );

  $regions = array(
    'header_left',
    'header_main',
    'header_right',
    'menu_bar',
    'sidebar_first',
    'content',
    'sidebar_second',
    'footer',
    'content_top',
    'content_left',
    'content_right',
    'content_bottom',
  );

  $form['unused'] = array(
    '#theme_wrappers' => array('cp_layout_unused_widgets'),
    '#tags' => os_boxes_get_tags(),
    '#factories' => os_boxes_get_factories(),
  );

  $form['layout'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'cp_layout').'/theme/cp_layout.js',
      ),
      'css' => array(
        drupal_get_path('module', 'cp_layout').'/theme/cp_layout.common.css',
      ),
    ),
    '#pre_render' => array(
      '_cp_layout_rearrange_content_regions',
    ),
  );

  if ($use_content_regions) {
    $form['layout']['#attributes'] = array(
    	'class' => array(
        'content-regions',
      ),
    );
  }

  foreach ($regions as $r) {
    $form['layout'][$r] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'cp-region',
          drupal_html_class('cp-region-'.$r),
        ),
      ),
      'widgets' => array(
        '#type' => 'hidden',
        '#default_value' => '',
      ),
    );
  }

  foreach ($blocks as $bid => $b) {

    if ($b['region'] == -1 || $b['region'] === FALSE) {
      $form['unused'][$bid] = array(
        '#theme' => 'cp_layout_widget',
        '#widget' => $b,
      );
    }
    elseif (isset($form['layout'][$b['region']])) {
      $form['layout'][$b['region']][$bid] = array(
        '#theme' => 'cp_layout_widget',
        '#widget' => $b,
      );
    }
  }

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
    'cancel' => array(
      '#type' => 'button',
      '#value' => t('Cancel'),
    ),
  );

  $form['#submit'][] = 'cp_layout_full_submit';

  return $form;
}

function cp_layout_ajax() {

}

function _cp_layout_rearrange_content_regions($elements) {
  foreach ($elements as $r => $region) {
    if (strpos($r, 'content_') !== FALSE) {
      unset($elements[$r]);
      $elements['content'][$r] = $region;
    }
  }

  return $elements;
}

function cp_layout_full_submit($form, &$form_state) {
  dpm($form_state['values']);
  $regions = $form_state['values']['layout'];
  $ctxs = array('os_public', $form_state['values']['context']);
  $blocks = os_layout_get_multiple($ctxs, false, true);
  foreach ($regions as $r => $d) {
    $widgets = explode('|', $d['widgets']);
    $weight = 0;
    foreach ($widgets as $w) {
      if (isset($blocks[$w])) {
        $blocks[$w]['region'] = $r;
        $blocks[$w]['weight'] = $weight++;
      }
    }
  }

  os_layout_set_multiple($ctxs, $blocks);
}