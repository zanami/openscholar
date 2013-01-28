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
    $form['schema_type'] = array(
      '#type' => 'select',
      '#title' => t('Schema type'),
      '#description' => t('Display files by the schema type.'),
      '#default_value' => $this->options['schema_type'],
      '#options' => array(
        'private' => t('Private'),
        'public' => t('Public'),
      ),
    );
  }

  /**
   * Add to the query conditions the file schema type.
   */
  function query($group_by = FALSE) {
    $this->query->add_where($group_by, 'uri', $this->options['schema_type'] . "://%", 'LIKE');
  }
}
