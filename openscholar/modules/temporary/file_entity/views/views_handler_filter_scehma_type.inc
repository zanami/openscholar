<?php
/**
 * @file
 *
 * Filter by the file schema type.
 */

class views_handler_filter_scehma_type extends views_handler_filter {

  /**
   * Set default field name to render.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['schema_type'] = array('default' => 'private');
    return $options;
  }

  /**
   * Define the file schema type.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Collect the available schema types.
    $options = array();
    foreach (file_get_stream_wrappers() as $type => $info) {
      $options[$type] = $info['name'];
    }

    $form['schema_type'] = array(
      '#type' => 'select',
      '#title' => t('Schema type'),
      '#description' => t('Display files by the Schema type.'),
      '#default_value' => $this->options['schema_type'],
      '#options' => $options,
    );
  }

  /**
   * Add to the query conditions the file schema type.   */
  function query($group_by = FALSE) {
    $this->query->add_where($group_by, 'uri', $this->options['schema_type'] . "://%", 'LIKE');
  }
}
