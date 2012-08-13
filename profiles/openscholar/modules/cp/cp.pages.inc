<?php

function cp_feature_customize($js, $feature_name) {
  if (!$js) {
    drupal_goto('features/'. $feature_name);
    return;
  }

  ctools_include('modal');
  ctools_include('ajax');
  $feature = feature_load($feature_name);
  $feature_title = features_get_feature_title($feature);

  $form_state = array(
    'title' => $feature_title,
    'ajax' => TRUE,
  );

  // We should have all feature form settings named ${feature_name}_settings
  // but just in case
  if ($item = menu_get_item('features/'. $feature_name)) {
    // the page callback should be drupal_get_form and the first arg is form_id
    $form_id = array_shift($item['page_arguments']);
    // if any file needs to be included (usually ${feature_name}.pages.inc)
    if ($file = $item['include_file']){
      require_once($file);
    }

    if(count($item['page_arguments'])){
    	$form_state['args'] = $item['page_arguments'];
    }

    $output = ctools_modal_form_wrapper($form_id, $form_state);

    if (empty($output)) {
      $output = array();
      if (isset($_REQUEST['refresh']) && $_REQUEST['refresh'] == 'yes'){
        $output[] = ctools_ajax_command_reload();
      }
      else {
        $output[] = ctools_modal_command_dismiss();
      }
    }
    else {
      $output[] = ajax_command_append('#modal-content div.buttons','<input class="form-submit close" onClick="$(\'div.modal-header a.close\').click(); return false;" value="Cancel" />');
      $output[] = ajax_command_after('#modal-content #edit-reset','<input class="form-submit close" onClick="$(\'div.modal-header a.close\').click(); return false;" value="Cancel" />');
    }
  }
  else {
    $output[] = ctools_modal_command_dismiss();
  }

  echo ajax_render($output);
  exit;
}