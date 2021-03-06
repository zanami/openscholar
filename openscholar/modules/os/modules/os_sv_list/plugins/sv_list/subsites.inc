<?php 

/**
 * @file subsites.inc
 * 
 * subsites plugin for sv_list box.  let's try and keep this applicable to files AND nodes
 */

$plugin = array(
  'handler' => array('class' => 'sv_list_subsites'),
  'entity_type' => array(),
  'bundle' => array(),
);

class sv_list_subsites extends sv_list_plugin  {
  
  public function options_defaults() {
    return array('query_scope' => 'current');
  }
  
  public function options_form($options, &$form) {
    if (!module_exists('vsite') || ($vsite = spaces_get_space()) == FALSE) {
      return;
    }
    
    if (!in_array($vsite->og->type, variable_get('subsite_parents', array()))) {
      return;
    }
    
    ctools_include('subsite','vsite');
    $form['filters']['query_scope'] = array(
      '#title' => 'Include results from',
      '#type' => 'checkboxes',
      '#options' => array('current' => "Current Site", 'subsites' => 'Subsites'),
      '#default_value' => $options['query_scope'],
      '#access' => vsite_has_subsites(),
      '#required' => TRUE,
    );
    //how to alter the display style?  do we need a separate alter hook?
  }
  
  /**
   * @function query_alter
   * 
   * Limit the query's scope to items in this subsite.
   */
  public function query_alter($options, &$efq) {
    if (!module_exists('vsite') || (($vsite = spaces_get_space()) == FALSE)) {
      return;
    }
    
    //skip this if subsites aren't an option
    if (!in_array($vsite->og->type, variable_get('subsite_parents', array()))) {
      return;
    }
      
    // Filter based on site or subsite.
    $gids = array();
    $scope = array_filter($options['query_scope']);
    $scope = (empty($scope)) ? array('current') : array_keys($scope);
    
    //remove default og group ref condition
    $efq->fieldConditions = array_filter($efq->fieldConditions, function($a) {
      return ($a['field']['field_name'] != OG_AUDIENCE_FIELD);
    });
    
    //add current
    if (in_array('current', $scope)) {
      $gids[$vsite->id] = $vsite->id;
    }
    
    //add subsite
    if (in_array('subsites', $scope)) {
      ctools_include('subsite','vsite');
      $gids = array_merge($gids,vsite_get_subsites());
    }

    $efq->fieldCondition(OG_AUDIENCE_FIELD, 'target_id', $gids, 'IN');
  }
}
