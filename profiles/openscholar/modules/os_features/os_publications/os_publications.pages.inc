<?php

function os_publications_overview() {
  $args = func_get_args();

  if (count($args) == 0) {
    // get default types the user has selected
    $defaults = variable_get('os_publications_default_types', array());
    if (count($defaults)) {
      $args = array('type', $defaults);
    }
  }

  module_load_include('inc', 'biblio', 'includes/biblio.pages');

  return call_user_func_array('biblio_page', $args);
}