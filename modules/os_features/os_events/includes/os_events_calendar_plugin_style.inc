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
    dpm('ok!');
    $i = 0;
    foreach ($this->view->argument as $name => $handler) {
      if (date_views_handler_is_date($handler, 'argument')) {
        $this->date_info->date_arg_pos = $i;
        return $handler;
      }
      $i++;
    }
    return FALSE;
  }

}
