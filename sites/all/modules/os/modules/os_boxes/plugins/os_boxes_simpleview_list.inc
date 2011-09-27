<?php

/**
 * @file
 * Extends "Simpleview" list to include os caching params that simpleviews should not care about.
 */

class os_boxes_simpleview_list extends spaces_simpleviews_post_list {

	/*
   * Define the default cache type for os_boxes
   */
  public $cache_type = BLOCK_CACHE_CONTENT_CACHE;
  
  /*
   * Define the default region for this box.
   */
  public $region;
  
  /**
   * Return the cache parameters for this box
   */
  public function cache_info(){
    
    //Clear when bio nodes are saved
    $cache_key = array(
      'os_og' => array('current' => 'current'),
      'node' => array('biblio' => 'biblio'),
    );
    
    //Add based on simpleview filters
    $filter = $this->options['filter'];
    if (strstr($filter, 'node') !== FALSE && $type = end(explode(':', $filter))) {
      $cache_key['node'] = array($type => $type);
    }
    
    return array('type' => BLOCK_CACHE_CONTENT_CACHE, 'keys' => $cache_key);
  }
  
}