<?php

/**
 * Searches through textareas and file fields for references to this file
 *
 * @param $file: a fully loaded file object
 */
function _os_files_install_find_nodes($file) {
  $fields = &drupal_static(__FUNCTION, array());
  $file_field_types = array(
    'file',
    'imagefield_crop',
  );
  $text_field_types = array(
    'text_with_summary',
    'text_long',
  );

  if (empty($fields)) {
    $q = db_select('field_config', 'fc')
      ->fields(array('field_name', 'field_type'))
      ->condition('active', 1)
      ->condition('deleted', 0)
      ->condition('field_type', $file_field_types + $text_field_types)
      ->execute();

    foreach ($q as $r) {
      $fields[$r->field_name] = $r->field_type;
    }
  }

  $eids = array();
  foreach ($fields as $f => $t) {
    $table = 'field_data_'.$f;

    $q = db_select($table, 'ft')
      ->fields(array('entity_type', 'entity_id'));
    if (in_array($t, $file_field_types)) {
      $q->condition($f.'_fid', $file->fid);
    }
    elseif (in_array($t, $text_field_types)) {
      $q->condition($f.'_format', 'filtered_html');
      $filename = basename($file->uri);
      $or = db_or()
        ->condition($f.'_value', '%"fid":"'.$file->fid.'"%', 'LIKE')
        ->condition($f.'_value', '%data-fid="'.$file->fid.'"%', 'LIKE')
        ->condition($f.'_value', '%'.$filename.'%', 'LIKE');
      $q->where($or);
    }
    $q = $q->execute();

    foreach ($q as $r) {
      $eids[$r->entity_type][] = $r->entity_id;
    }
  }

  return $eids;
}

function _os_files_install_find_space($file, $eids) {
  $current_space = null;
  if (isset($file->{OG_AUDIENCE_FIELD}[LANGUAGE_NONE][0]['target_id'])) {
    $current_space = $file->{OG_AUDIENCE_FIELD}[LANGUAGE_NONE][0]['target_id'];
  }


}