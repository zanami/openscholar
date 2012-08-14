<?php

class vsite_handler_field_node extends views_handler_field_node {
  /**
   * Override of option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['vsite'] = array('default' => array(
        'frontpage' => FALSE,
      ));
    return $options;
  }

  /**
   * Override of options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

      $form['vsite']['frontpage'] = array(
        '#title' => t('Link to vsite frontpage'),
        '#description' => t('Link a group vsite to its frontpage.'),
        '#type' => 'checkbox',
        '#checked' => !empty($this->options['vsite']['frontpage']),
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array(
          'edit-options-link-to-node' => array(1),
        ),
      );
  }

  /**
   * Override of render_link().
   * Change link path if 'frontpage' option is enabled. Note that
   * the url is generated beforehand.
   */
  function render_link($data, $values) {
    parent::render_link($data, $values);
    
    //Is the link set on this node?
    if ($data !== NULL && $data !== '' && !empty($this->options['vsite']['frontpage'])  && isset($values->_field_data['nid']['entity']->field_group_path)) {
      
      //Get the groups link field and render it if avalible
      $items = field_get_items('node', $values->_field_data['nid']['entity'], 'field_group_path');
      if(count($items)){
        $path = theme('link_formatter_link_plain', array('element' => array_shift($items)));
        
        //If a valid path was stored in the field the use that
        if($path){
          $this->options['alter']['path'] = $path;
          $this->options['alter']['absolute'] = true;
        }
      }
    }
    return $data;
  }
}
