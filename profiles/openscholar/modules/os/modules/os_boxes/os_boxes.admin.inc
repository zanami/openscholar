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

function os_boxes_delete_widget($box) {
  $form_state = array(
    'box' => $box,
  );

  $form = drupal_build_form('os_boxes_delete_widget_form', $form_state);

  return $form;
}

function os_boxes_delete_widget_form($form_id, $form_state) {
  $box = $form_state['box'];
  $form = array();

  $form['bid'] = array(
    '#type' => 'value',
    '#value' => $box->delta,
  );

  $form['warning'] = array(
    '#markup' => t('Are you sure you want to delete this widget? This will delete the widget and remove it from all pages
    it is currently placed on. This action is cannot be undone.'),
  );
  $form['actions'] = array(
    '#type' => 'actions',
    'yes' => array(
      '#type' => 'submit',
      '#value' => t('Yes'),
    ),
    'no' => array(
      '#type' => 'submit',
      '#value' => t('No'),
    ),
  );

  $form['#submit'][] = 'os_boxes_widget_delete_submit';

  return $form;

}

function os_boxes_widget_delete_submit($form, &$form_state) {
  if ($form_state['values']['op'] == t('Yes')) {
    if (module_exists('spaces') && $space = spaces_get_space()) {
      $space->controllers->boxes->del($form_state['values']['bid']);
    }
    else {
      if ($box = boxes_box_load($form_state['values']['bid']))
        $box->delete();
     module_load_include('inc', 'block', 'block.admin');
     block_custom_block_delete_submit($form, $form_state);
    }
    unset($form_state['redirect']);
    $form_state['no_redirect'] = true;

    overlay_commands_add(ajax_command_remove('#boxes-'.$form_state['values']['bid']));
  }
  overlay_commands_add(overlay_command_dismiss());
}