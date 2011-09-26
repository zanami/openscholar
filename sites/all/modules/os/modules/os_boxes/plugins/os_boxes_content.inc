<?php

/**
 * @file
 * The base class for OS content widgets. It sets the default for content related boxes
 */
abstract class os_boxes_content extends os_boxes_default {
	
	/*
   * Define the default cache type for os_boxes
   */
  public $cache_type = BLOCK_CACHE_CONTENT_CACHE;
  
  /**
   * Return the cache key for this box
   *
   * This should be extended to provide a more specific cache key if applicable
   */
  public function cache_keys(){
    //The default for all os_boxes is to clear the cache of this box each time the any node on the site is saved
    return array(
      'os_og' => array('current' => 'current'),
    );
  }
  
}
