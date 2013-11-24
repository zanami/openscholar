<?php 

/**
 * @file plugin.inc
 * 
 * Parent class for sv list plugins
 */

$plugin = array(
  'handler' => array('class' => 'sv_list_plugin'),
  'entity_type' => array(),
  'bundle' => array(),
);

class sv_list_plugin {
  function __construct($entity_type, $bundle) {
    $this->entity_type = $entity_type;
    $this->bundle = $bundle;
  }
  
  function option_defaults() {  
  }
}