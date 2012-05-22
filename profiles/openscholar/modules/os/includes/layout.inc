<?php

function testmerichard(){
  os_layout_get_multiple(array(
      'os_public',
      'announcements_announcements'
  ));
}

/**
 * Returns the blocks for a individual context
 *
 * @param string $context
 * The identifier string for a context
 *
 * @return array
 * List of blocks
 */
function os_layout_get($context){
  /**
   * Context load alters the context (for vsites) so no need to do anything extra
   */
  $o_context = context_load($context);
  
  if (is_object($o_context) && isset($o_context->reactions['block'])) {
    if (array_key_exists('blocks', $o_context->reactions['block'])) {
      return $o_context->reactions['block']['blocks'];
    }
    else {
      //No blocks set yet for this context
      return array();
    }
  }
  
  //This was an invalid context
  return false;
}

/**
 * Returns the blocks for aggregated contexts child contexts overriding the parents
 *
 * @param array $contexts
 * A list of context identifiers to load and aggregate
 *
 * @return array
 * List of blocks
 */
function os_layout_get_multiple($contexts){
  usort($contexts, '_os_context_sort_by_hierarchy');
  
  $context_blocks = array();
  
  //Retrieve all of the blocks
  foreach ( $contexts as $context ) {
    //Merge the blocks with child blocks replacing parent blocks
    $blocks = os_layout_get($context);
    
    if (is_array($blocks)) {
      $context_blocks = array_merge($context_blocks, $blocks);
    }
  }
  
  return $blocks;
}

/**
 * Saves an array blocks for a individual context
 *
 * @param string $context
 * @param array $blocks
 *
 * @return boolean Success
 */
function os_layout_set($context, $blocks){
  if (module_exists('vsite_layout')) {
    //Write the context to the overrides table
    vsite_layout_context_set($context, $blocks);
  }
  else {
    $o_context = context_load($context);
    $o_context->reactions['block']['blocks'] = $blocks;
    //write the context to the database
    context_save($context);
  }
}

/**
 * Saves an array blocks to multiple contexts, resolving weight issues and deciding what overrides need to be made
 *
 * @param array $contexts
 * @param array $blocks
 *  An array of blocks to save to the contexts with the correct block keys
 *
 * @return boolean Success
 */
function os_layout_set_multiple($contexts, $blocks){
  usort($contexts, '_os_context_sort_by_hierarchy');
  
  $removed_blocks = array();
  
  //Fields to check for changes
  $block_fields = array('weight', 'region');
  
  $context_settings = array();
  //Resolve dependencies and alter the contexts accordingly
  foreach ( $contexts as $context ) {
    $layout_blocks = os_layout_get($context);
    
    //Get the removed blocks that no longer exist from this context
    $removed_blocks = array_merge(array_diff(keys($layout_blocks), keys($blocks)), $removed_blocks);
    
    //check common blocks for differences
    foreach ( array_intersect_key($layout_blocks, $blocks) as $bid ) {
      $changed = false;
      foreach ( $block_fields as $key ) {
        if ($layout_blocks[$bid][$key] != $blocks[$bid][$key]) {
          $changed = true;
          /**
           * @todo Adapt block weights and check for removed bocks
           */
          switch ($key){
            case 'region':
              
              break;
            case 'weight':
              
              break;
          }
        }
      }
      //Not changed take no action
      if (! $changed) {
        unset($blocks[$bid]);
      }
    }
    
    $context_settings[$context] = $layout_blocks;
  }
  
  //Remaining block overrides belong in the child-most context
  $context_settings[$context] = array_merge($context_settings[$context],$blocks);
  
  //Commit the context settings
  foreach ( $context_settings as $context => $layout_blocks ) {
    os_layout_set($context, $layout_blocks);
  }
}

/**
 * Takes a single or multiple contexts and returns all unused blocks for that context.
 * This is an aggregation of blocks that have been removed from the default contexts, or do not have a region,
 * and box instances that have been created.
 *
 * @param array/string $contexts
 * One or more contexts to load blocks from
 *
 * @return array
 * Blocks that are not used, with category info
 */
function os_layout_get_unused_blocks($contexts){

}

/**
 * Adds/Updates an individual block within a named context
 *
 * @param array $block
 * @param string $context
 *
 * @return boolean Success
 */
function os_layout_put($block, $context){
  $context_blocks = os_layout_get($context);
  //Valid Context?
  if (is_array($context_blocks)) {
    //Add new block settings
    $context_blocks[$block['module'] . '-' . $block['delta']] = $block;
    
    os_layout_set($context, $context_blocks);
  }
}

/**
 * Return a context ranking
 */
function _os_context_get_rank($context){
  $context_rank = drupal_static(__FUNCTION__, array());
  
  if (! isset($context_rank[$context])) {
    //Global Special Contexts
    if (in_array($context, array(
        'os_public',
        'os_cp'
    ))) {
      $context_rank[$context] = 1;
      return $context_rank[$context];
    }
    
    $map = features_get_component_map('context');
    if (array_key_exists($context, $map)) {
      //Feature context
      $context_rank[$context] = 2;
    }
    else {
      //Page or other non-feature context
      $context_rank[$context] = 3;
    }
  }
  
  return $context_rank[$context];
}

/**
 * Sort a list of contexts by hierarchy
 *
 * @param string $context1
 * @param string $context2
 */
function _os_context_sort_by_hierarchy($context1, $context2){
  $a = _os_context_get_rank($context1);
  $b = _os_context_get_rank($context2);
  
  if ($a == $b) {
    return 0;
  }
  return ($a < $b) ? - 1 : 1;
}