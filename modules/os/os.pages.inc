<?php

/**
 * Default front page, stub
 */
function os_frontpage(){
  
  return "I AM THE HOME PAGE";
}

function os_page_not_found_page() {
  $t = '<strong>' . t('This page can not be found.') . '</strong>';
  $t = t('The URL might be incorrect or have changed.  Please go back to the !link', 
      array('!link' => l('homepage', '<front>')));
  return $t;
}