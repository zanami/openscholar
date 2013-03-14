<?php

/**
 * @file
 * Views style plugin for the OS events module.
 */

/**
 * Default style plugin to render an iCal feed.
 */
class os_events_calendar_plugin_style extends calendar_plugin_style {


  /**
   * Helper function to find the date argument handler for this view.
   */
  function date_argument_handler() {
    $i = 0;
    foreach ($this->view->argument as $name => $handler) {
      if (date_views_handler_is_date($handler, 'argument')) {
        // Change the handler argument to be date.
        $handler->argument = !empty($_GET['date']) ? $_GET['date'] : date('Y-m', time());

        $handler->date_range = $handler->date_handler->arg_range($handler->argument);
        $handler->min_date = $handler->date_range[0];
        $handler->max_date = $handler->date_range[1];

        // Set calendar type from query string.
        $this->options['calendar_type'] = !empty($_GET['type']) ? $_GET['type'] : 'month';
        return $handler;
      }
      $i++;
    }
    return FALSE;
  }
}
