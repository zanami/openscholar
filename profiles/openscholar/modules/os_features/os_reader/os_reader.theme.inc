<?php 


/**
 * Theme function for 'os_reader_google_feedapi' link field formatter.
 */
function theme_os_reader_formatter_os_reader_google_feedapi($info) {
  
  $feed_url = $info['element']['url'];
  $num_to_show = $info['num_to_show'];
  
  return check_plain("Show [{$feed_url}] with max [{$num_to_show}]");
}