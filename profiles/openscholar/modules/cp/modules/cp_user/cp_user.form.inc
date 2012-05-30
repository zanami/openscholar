<?php

/**
 * Load the users control panel
 */
function cp_user_members(){
  $vsite = spaces_get_space();
  
  if($vsite){
    $view = views_get_view('os_site_members', 'default');
    $view->set_arguments(array('node', $vsite->id));
    $view->override_url = $_GET['q'];
    return $view->preview();
  }
  
}

/**
 *
 */
function cp_user_add_user(){
  
  
}