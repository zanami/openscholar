<?php

/**
 * @file
 * Handles incoming requests to fire off regularly-scheduled tasks
 */

include_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
os_cron_run();
