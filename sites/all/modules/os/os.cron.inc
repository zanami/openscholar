<?php

/**
 * Perform routine maintenance tasks so full system cron can run less often
 */
function _os_cron_run() {
  
	//List of the cache tables to clear regularly
//  $cache_tables = array();
//
//  foreach ($cache_tables as $table) {
//    cache_clear_all(NULL, $table);
//  }
  
  $cache_lifetime = variable_get('page_cache_lifetime', 0);
  if($cache_lifetime){
    //Expire old items
    cache_clear_all(NULL, 'cache_page');
  }else{
    //Truncate the cache table
    cache_clear_all('*', 'cache_page', true);
  }
  
}