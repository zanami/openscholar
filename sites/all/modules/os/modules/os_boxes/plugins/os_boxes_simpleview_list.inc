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
   * Return the cache key for this box
   *
   * This should be extended to provide a more specific cache key if applicable
   */
  public function cache_keys(){
    //The default for all os_boxes is to clear the cache of this box each time the any node on the site is saved
    $cache_key = array(
      'os_og' => array('current' => 'current'),
    );
    
    //Add based on simpleview filters
    $filter = $this->options['filter'];
	  if (strstr($filter, 'node') !== FALSE && $type = end(explode(':', $filter))) {
	    $cache_key['node'] = array($type => $type);
	  }
	  
	  return $cache_key;
  }
  
  
}