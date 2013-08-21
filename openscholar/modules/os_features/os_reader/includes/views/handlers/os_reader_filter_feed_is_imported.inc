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
    $input_value = $this->view->exposed_raw_input['feed_is_imported'];
    $table = 'field_data_field_vsites_reference';
    $field = 'field_vsites_reference_target_id';

    if (empty($input_value)) {
      // 'All' is selected.
      return;
    }

    $db = db_or();

    // Adding relation ship for the table of the field.
    $this->query->add_table($table, $this->relationship);

    // Input value is 1 on 'Imported', 2 on 'Not imported'.
    $operator = $input_value == 1 ? 'IN' : 'NOT IN';

    // Alter the query.
    $this->query->add_where($this->options['group'], "{$table}.{$field}", array(1), $operator);

    return;
  }

  function admin_summary() {
    return t('Is Imported');
  }
}
