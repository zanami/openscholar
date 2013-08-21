<?php

/**
 * @file
 * Filtering a file with by the real file name or file title.
 */

/**
 * Filter feed items by whether or not they were imported to VSite.
 */
class os_reader_filter_feed_is_imported extends views_handler_filter {
  function value_form(&$form, &$form_state) {
    $values = array();
    $form['value'] = array(
      '#type' => 'select',
      '#title' => t('Status'),
      '#options' => array(t('All'), t('Imported'), t('Not Imported')),
    );
  }

  function query($group_by) {
//    if (empty($this->view->exposed_raw_input['file_name_title'])) {
//      return;
//    }
//
//    $db = db_or();
//    foreach (array('filename', 'uri') as $field) {
//      $db->condition($field, "%" . $this->view->exposed_raw_input['file_name_title'] . "%", 'LIKE');
//    }
//
//    $this->query->add_where($group_by, $db);
    return;
  }

  function admin_summary() {
    return t('Is Imported');
  }
}
