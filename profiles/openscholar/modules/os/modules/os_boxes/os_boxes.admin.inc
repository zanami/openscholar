<?php
function os_boxes_edit_widget($box, $source = '') {
  if (!$box) {
    // TODO: Multi-step form to choosen plugin type?
    return MENU_NOT_FOUND;
  }

  $form_state = array(
    'box' => $box,
    'source' => $source,
  );

  $form = drupal_build_form('boxes_box_form', $form_state);

  unset($form['submit']['#attributes']);
  unset($form['cancel']['#attributes']);

  return $form;
}

function os_boxes_form_boxes_box_form_alter(&$form, &$form_state) {
  $form['#submit'][] = 'os_boxes_widget_submit';
}

/**
 * Generate the HTML for our widget and pass it as an ajax command
 * Through overlay_commands
 */
function os_boxes_widget_submit($form, $form_state) {
  $box = boxes_box_load($form_state['box']->delta);
  $html = '';
  $target = '';
  $op = 'prepend';
  switch ($form_state['source']) {
    case 'cp-layout':
      $b = os_boxes_os_widget($box);
      drupal_alter('os_widget', $b);
      $r = array('#theme' => 'cp_layout_widget', '#widget' => $b);
      $html = drupal_render($r);
      $target = '.widget-container';
    break;
    case 'inline':
      $block = boxes_block_view($box->delta);
      $t = _block_render_blocks(array($block));
      $html = theme('block', $t[0]);
      $target = '.widget-container';  // TODO: replace with the real thing
    break;
  }
  if (!(isset($form_state['box']->new) && $form_state['box']->new)) {
    $target = '#boxes-'.$box->delta;
    $op = 'replace';
  }

  $command = 'ajax_command_'.$op;
  $command = $command($target, $html);

  overlay_commands_add($command);
  ctools_include('modal');
  overlay_commands_add(ctools_modal_command_dismiss());  // overlay_close_dialog doesn't work because the form resets it
}