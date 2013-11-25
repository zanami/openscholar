<?php

/**
 * @file
 * Form-related functions for module `os_tour`.
 */

/**
 * After build callback `cp_layout` form. Adds "Take a tour" link and assets.
 *
 * @see os_tour_form_cp_layout_full_form_alter()
 */
function os_tour_form_cp_layout_after_build($form) {
  // Only continues if we have the expected markup to attach to.
  if (!isset($form['context_selection'])) {
    return;
  }

  // Attaches CSS, JS, and library assets.
  $path = drupal_get_path('module', 'os_tour');
  $form['context_selection']['#attached']['css'] = array(
    "$path/theme/os_tour.cp_build_layout.css",
  );
  $form['context_selection']['#attached']['library'] = array(
    array('os_tour', 'hopscotch'),
  );
  $form['context_selection']['#attached']['js'] = array(
    "$path/theme/os_tour.cp_build_layout.js",
  );

  // Creates and adds tour as prefix markup, or prepends to existing markup.
  $original = '';
  if (isset($form['context_selection']['#prefix'])) {
    $original = $form['context_selection']['#prefix'];
  }
  $build = array(
    '#title' => t('Take a tour'),
    '#type' => 'link',
    '#href' => '',
    '#options' => array(
      'attributes' => array(
        'id' => 'os-tour-cp-build-layout',
        'class' => array('os-green-button'),
      ),
    ),
  );
  $markup = drupal_render($build);
  $form['context_selection']['#prefix'] = $markup . $original;

  return $form;
}
