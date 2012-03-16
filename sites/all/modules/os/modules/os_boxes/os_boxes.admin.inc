<?php
function os_boxes_edit_widget($box) {
  if (!$box) {
    // TODO: Multi-step form to choosen plugin type?
    return MENU_NOT_FOUND;
  }

  $form_state = array(
    'box' => $box,
  );

  $form = drupal_build_form('boxes_box_form', $form_state);

  unset($form['submit']['#attributes']);
  unset($form['cancel']['#attributes']);

  return $form;
}