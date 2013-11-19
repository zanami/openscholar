<?php 

/**
 * @file taxonomy.inc
 * 
 * taxonomy plugin for sv_list box.  let's try and keep this applicable to files AND nodes
 */

$plugin = array(
  'handler' => array('class' => 'sv_list_taxonomy'),
  'entity_type' => array(),
  'bundle' => array(),
);

class sv_list_taxonomy extends sv_list_plugin {
  public function options_defaults() {
    //@todo document these.  I don't know what they all mean, especially the only_ vars
    return array(
      'vocabs' => array(),
      'terms' => array(),
      'only_vocabs' => TRUE,
      'only_vocabs_values' => array(),
    );
  }
  
  public function options_form($options) {
    // Get all relevant vocabularies.
    $vocabs = os_sv_list_get_content_type_vocabs($options['content_type'], $this->entity_type); //@TODO does this func work with files terms too?

    if (empty($vocabs)) {
      return;
    }
        
    // The 'vocabs' tree will contain select fields - one for each vocabulary.    
    $form['vocabs'] = array(
      '#type' => 'item',
      '#prefix' => '<div id="vocabs">',
      '#suffix' => '</div>',
      '#title' => t('Filter by vocabulary'),
      '#description' => t('Only include posts tagged with terms from the following vocabularies.'),
      '#tree' => TRUE,
    );  
    
    // Create a select field for each vocabulary, with the vocabulary's terms as the field's options.
    foreach ($vocabs as $vid => $vocab) {
      // Get the vocabulary's terms.
      $terms = array();      
      foreach (taxonomy_get_tree($vid) as $item) {
        $term = taxonomy_term_load($item->tid);
        $terms[$term->tid] = $term->name;
      }
      
      $element = 'vocab_' . $vid;
      $vocab_default_value = (isset($options['vocabs'][$element])) ? $options['vocabs'][$element] : array();
      
      // Create select field with the terms as options.
      $form['vocabs'][$element] = array(
        '#type' => 'select',
        '#options' => $terms,
        '#title' => $vocab,
        '#multiple' => TRUE,
        '#default_value' => $vocab_default_value,
        '#element_validate' => array('os_sv_list_terms_element_validate'), //group selected terms from all vocab fields
      );
    }
    
    return $form;
  }
  
  /**
   * @function query_alter
   * 
   * Limit the query's scope to items in this subsite.
   */
  public function query_alter($options, &$efq) {
    // For all Queries but image, filter out based on vocab.
    if ($options['terms'] && $options['terms'] != -1) {
      $ids = $this->tagged_entities($options);
      if (is_array($ids)) {
        $entity_info = entity_get_info($this->entity_type);
        $table = $entity_info['base table'];
        $id = $entity_info['entity keys']['id'];
        
        $efq->propertyCondition($id, $ids, 'IN');
      }
    }
  }
  
  /**
   * @function tagged_nodes
   * 
   * Return the nodes attached to terms of the selected vocabulary.
   */
  private function tagged_entities($options) {
    // Get the  terms the user selected.
    $tids = (is_array($options['terms'])) ? $options['terms'] : drupal_map_assoc($options['terms']);
    
    // No terms were found.
    if (empty($tids)) {
      return;
    }
    
    $bundles = os_get_bundles(array(1)) + os_files_get_bundles();
    $content_types = ($options['content_type'] == 'all') ? array_keys($bundles) : array($options['content_type']);
    
    $query = db_select('field_data_og_vocabulary', 'v')
      ->fields('v', array('entity_id'))
      ->condition('v.entity_type', $this->entity_type)
      ->condition('v.bundle', $content_types, 'IN');
    
    //add an IN for each vocab.  item must be in all vocabs to show up.
    $vocabs = array_reduce(taxonomy_term_load_multiple($tids), function ($ret, $term) {
      $ret[$term->vid][] = $term->tid;
      return $ret;
    }, array());
    
    foreach ($vocabs as $vid => $tids) {
      $query->condition('v.og_vocabulary_target_id', $tids, 'IN');
    }

    $result = $query->execute();
    return ($result->rowCount()) ? $result->fetchCol() : array(0); //needs a nonempty array or we get db errors
  }
}
