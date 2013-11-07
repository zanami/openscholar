<?php 

/**
 * @file node_biblio.inc
 * 
 * Biblio plugin for sv_list box
 */

$plugin = array(
  'handler' => array('class' => 'sv_list_node_biblio'),
  'entity_type' => array('node'),
  'bundle' => array('biblio'),
);

class sv_list_node_biblio {
  public function __construct() {
    $this->biblio_types = $this->_biblio_types();
  }
  
  public function options_defaults() {
    return array(
      'biblio_whitelist' => drupal_map_assoc(array_keys($this->biblio_types)), 
    );
  }
  
  public function options_form($box_options) {
    $form['biblio_whitelist'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Select publication types to display'),
      '#default_value' => $box_options['biblio_whitelist'], //isset($this->options['biblio_whitelist']) ? $this->options['biblio_whitelist'] : $defaults['biblio_whitelist'],
      '#options' => $this->biblio_types,
      '#prefix' => '<div id="os_sv_list_biblio_whitelist">',
      '#suffix' => '</div>',
      '#attached' => array(
        'js' => array(
          drupal_get_path('module', 'os_boxes').'/misc/selectAll.js',
        ),
      ),
      '#states' => array(
        'visible' => array(
          '#os_sv_list_content_type' => array('value' => 'biblio'),
        ),
        'invisible' => array(
          '#os_sv_list_content_type' => array('!value' => 'biblio'),
        ),
      )
    );
    
    return $form;
  }
  
  
  private function _biblio_types() {
    $rows = db_select('biblio_types', 'bt')
      ->fields('bt', array('tid', 'name'))
      ->condition('tid', 0, '>')
      ->condition('visible', 1)
      ->execute();
    $options = array('all' => 'Select All');
    foreach ($rows as $r) {
      $options[$r->tid] = $r->name;
    }
  
    return $options;
  }
}
