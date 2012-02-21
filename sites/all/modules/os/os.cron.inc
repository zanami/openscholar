<?php

/**
 * Perform routine maintenance tasks so full cron can run less often
 */
function os_openscholarcron() {
  
	//List of the cache tables to clear regularly
  $cache_tables = array('cache_page');
  
  //List of the hook _cron to be called regularly (skips those modules not enabled)
	$a_routine_cron_tasks = array(
	  'search', //Update drupal search index
	  'apachesolr', //Removes stale items from the index
	  'apachesolr_search', //Updates the apachesolr index
	  'apachesolr_attachments', //Adds attachments to the index
	  'feeds', //Retrieves new feed items
	  'job_scheduler', //scheduled jobs ie.(scholar_feed_importer)
	  'scholar_software', //Checks for new software releases
	);
	
	$args = array();
	foreach ($a_routine_cron_tasks as $module) {
		if(!module_exists($module)) continue; //This is not enabled
		
	  $function = $module .'_cron';
	  if(function_exists($function)){
      $result = call_user_func_array($function, $args);
	  }
	}
	
  foreach ($cache_tables as $table) {
    cache_clear_all(NULL, $table);
  }
}

/**
 * Executes a lightweight cron run when called
 * Used to perform maintenance tasks that need to take place regularly so full cron can run less often
 * @return
 * Returns TRUE if ran successfully
 */
function _os_cron_run() {
  // Try to allocate enough time to run all the hook_cron implementations.
  if (function_exists('set_time_limit')) {
    @set_time_limit(240);
  }

  // Fetch the cron semaphore
  $semaphore = variable_get('cron_semaphore', FALSE);

  if ($semaphore) {
    if (time() - $semaphore > 3600) {
      // Either cron has been running for more than an hour or the semaphore
      // was not reset due to a database error.
      watchdog('cron', 'Cron has been running for more than an hour and is most likely stuck.', array(), WATCHDOG_ERROR);

      // Release cron semaphore
      variable_del('cron_semaphore');
    }
    else {
      // Cron is still running normally.
      watchdog('cron', 'Attempting to re-run cron while it is already running.', array(), WATCHDOG_WARNING);
    }
  }
  else {
    if(function_exists('drupal_cron_cleanup')){
      // Register shutdown callback
      register_shutdown_function('drupal_cron_cleanup');
    }

    // Lock cron semaphore
    variable_set('cron_semaphore', time());

    // Iterate through the modules calling their openscholar cron handlers (if any):
    module_invoke_all('openscholarcron');

    // Record cron time
    variable_set('cron_last', time());
    watchdog('cron', 'Openscholar Cron run completed.', array(), WATCHDOG_NOTICE);

    // Release cron semaphore
    variable_del('cron_semaphore');

    // Return TRUE so other functions can check if it did run successfully
    return TRUE;
  }
}