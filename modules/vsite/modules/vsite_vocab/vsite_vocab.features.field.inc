<?php

/**
 * @file
 * vsite_vocab.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function vsite_vocab_field_default_fields(){
  $fields = vsite_vocab_get_vocab_fields();
  
  return $fields;
}

/**
 * Dynamically attach the vocab fields to all OS managed group content bundles
 * @return array
 */
function vsite_vocab_get_vocab_fields(){
  $fields = array();
  
  $bundles = array_keys(os_get_bundles(true, 'og', true));
  foreach ( $bundles as $bundle ) {
    
    // Exported field: 'ENTITY-BUNDLE-og_vocabulary'
    $fields["node-{$bundle}-og_vocabulary"] = array(
        'field_config' => array(
            'active' => '1', 
            'cardinality' => '-1', 
            'deleted' => '0', 
            'entity_types' => array(), 
            'field_name' => 'og_vocabulary', 
            'foreign keys' => array(
                'taxonomy_term_data' => array(
                    'columns' => array(
                        'target_id' => 'tid' 
                    ), 
                    'table' => 'taxonomy_term_data' 
                ) 
            ), 
            'indexes' => array(
                'target_id' => array(
                    0 => 'target_id' 
                ) 
            ), 
            'module' => 'entityreference', 
            'settings' => array(
                'handler' => 'base', 
                'handler_settings' => array(
                    'behaviors' => array(
                        'og_vocab' => array(
                            'status' => TRUE 
                        ), 
                        'views-select-list' => array(
                            'status' => 0 
                        ) 
                    ), 
                    'sort' => array(
                        'direction' => 'ASC', 
                        'field' => '', 
                        'property' => 'tid', 
                        'type' => 'none' 
                    ), 
                    'target_bundles' => array() 
                ), 
                'handler_submit' => 'Change handler', 
                'target_type' => 'taxonomy_term' 
            ), 
            'translatable' => '0', 
            'type' => 'entityreference' 
        ), 
        'field_instance' => array(
            'bundle' => $bundle, 
            'default_value' => NULL, 
            'default_value_function' => '', 
            'deleted' => '0', 
            'description' => '', 
            'display' => array(
                'default' => array(
                    'label' => 'above', 
                    'module' => 'og_vocab', 
                    'settings' => array(), 
                    'type' => 'og_vocab', 
                    'weight' => 2 
                ), 
                'teaser' => array(
                    'label' => 'above', 
                    'settings' => array(), 
                    'type' => 'hidden', 
                    'weight' => 0 
                ) 
            ), 
            'entity_type' => 'node', 
            'field_name' => 'og_vocabulary', 
            'label' => 'OG vocabulary', 
            'required' => 0, 
            'settings' => array(
                'behaviors' => array(
                    'prepopulate' => array(
                        'action' => 'hide', 
                        'fallback' => 'none', 
                        'skip_perm' => 'administer biblio', 
                        'status' => 0 
                    ) 
                ), 
                'user_register_form' => FALSE 
            ), 
            'widget' => array(
                'active' => 0, 
                'module' => 'og_vocab', 
                'settings' => array(), 
                'type' => 'og_vocab_complex', 
                'weight' => '2' 
            ) 
        ) 
    );
  }
  
  return $fields;
}
