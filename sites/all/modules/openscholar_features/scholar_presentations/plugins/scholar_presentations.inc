<?php 

/**
 * @class scholar_presentations
 * 
 * Extends os_simpleview to provide options for presentations
 */

class scholar_presentations extends os_simpleview_default {
  
  //add presented date to the form
  public function os_simpleview_form_display_options($options = NULL, $step = NULL){
    $form = parent::os_simpleview_form_display_options($options, $step);
    $form['sorts_fieldset']['sorts']['#options'] += array('presented_date' => t('Recently Presented'));
    return $form;
  }
    
  //override the default view
  function view_settings() {
    return (object)array('view_name' => 'scholar_presentations', 'display_id' => 'block_1');
  }
  
  //override and extendparse_options_srts
  protected  function _parse_options_sorts($sorting_method) {
    $override = array();
    switch ($sorting_method) {
      case 'presented_date':
        $override['sorts']['field_presentation_date_value'] = array(
          'order' => 'DESC',
          'delta' => -1,
          'id' => 'field_presentation_date_value',
          'table' => 'node_data_field_presentation_date',
          'field' => 'field_presentation_date_value',
          'relationship' => 'none',
          'override' => array(
            'button' => 'Override',
          ),
        );
      break;
       
      default:
        $override = parent::_parse_options_sorts($sorting_method);
      break;
        
    }
    return $override;
  }
}