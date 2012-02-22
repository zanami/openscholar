<?php

/**
 * Perform routine maintenance tasks so full system cron can run less often
 */
function _os_cron_run() {
  
	//List of the cache tables to clear regularly
  $cache_tables = array('cache_page');

  foreach ($cache_tables as $table) {
    cache_clear_all(NULL, $table);
  }
}