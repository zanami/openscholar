<?php

function cp_layout_full_form($form_state) {
  ctools_include('ajax');
  ctools_include('modal');

  $ctx = isset($form_state['build_info'])&&count($form_state['build_info']['args'])?$form_state['build_info']['args'][0]:'os_front';

  $use_content_regions = false;
  if ($ctx == 'os_front') {
    $use_content_regions = true;
  }
  $ctx = context_load($ctx);

  $form = array();
  if ($ctx === FALSE) return $form;  // context doesn't exist. bail out

  $plugin = context_get_plugin('reaction', 'block');

  // will need to be replaced if it doesn't work with spaces
  // no idea if it will or not
  $blocks = $plugin->get_blocks(null, $ctx);

  // filter out blocks we don't want
  // add info to existing blocks
  drupal_alter('layout_available_blocks', $blocks);


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
    '#tags' => array(),
    '#factories' => array()
  );

  $form['layout'] = array(
    '#type' => 'container',
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
    );
  }

  foreach ($blocks as $bid => $b) {
    // factory check
    if (0) {
      $form['unused']['#factories'][$bid] = $b;
    }

    if ($b->region == -1) {
      $form['unused'][$bid] = array(
        '#theme' => 'cp_layout_widget',
        '#widget' => $b,
      );
    }
    elseif (isset($form['layout'][$b->region])) {
      $form['layout'][$b->region][$bid] = array(
        '#theme' => 'cp_layout_widget',
        '#widget' => $b,
      );
    }

    if (isset($b->tags)) {
      $tags = explode(', ', $b->tags);
      foreach ($tags as $t) {
        if (!in_array($t, $form['unused']['#tags'])) {
          $form['unused']['#tags'][] = $t;
        }
      }
    }
  }

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