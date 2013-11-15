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
  
  /**
   * @function register_sorts()
   * 
   * Adds a custom sort for this entity/bundle
   */
  public function register_sorts() {
    return array('sort_biblio' => t('Year of Publication'));
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
  
  /**
   * @function query_alter 
   * 
   * Adds a filter to only use the selected biblio types.
   */
  public function query_alter($options, $efq) {
    $types = $this->_biblio_types();
    $whitelist = array_filter($options['biblio_whitelist']);
    
    if (count($types) > count($whitelist)) {
      $nids = db_select('biblio', 'b') 
        ->condition('biblio_type', array_values($whitelist), 'IN')
        ->fields('b', array('nid'))
        ->execute()
        ->fetchCol();  

      $efq->entityCondition('nid', $nids, 'IN');
    }
  }
  
  public function sort_biblio($options, $efq) {
    //borken!  biblio_year isn't a field so efq can't sort it.  
    $efq->fieldOrderBy('biblio_year', 'DESC'); 
    $efq->propertyOrderBy('created', 'DESC'); 
    $efq->addTag('biblio');
  }
  
  public function re_sort($options, $efq) {
    //if we can't figure out how to make the efq sort, maybe do a sort afterwards?  
    //not as ugly as it sounds, since we could just query for the 1-20 pubs we have to display on this page.
  }
}
