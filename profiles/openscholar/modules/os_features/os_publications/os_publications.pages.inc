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

  $ckey = _os_publications_cache_key($args);
  if ($cache = cache_get($ckey, 'cache_biblio_lists')) {
    return $cache->data;
  }

  module_load_include('inc', 'biblio', 'includes/biblio.pages');
  $output = call_user_func_array('biblio_page', $args);

  // handle caching
  cache_set($ckey, $output, 'cache_biblio_lists');

  return $output;
}

function _os_publications_cache_key($args) {
  $info = array(
    'func_args' => $args,
    'uri' => drupal_parse_url(request_uri()),
  );
  module_load_include('inc', 'biblio', 'includes/biblio.pages');
  $arg_list = biblio_arg_handler($info);

  $key = array();
  $key[] = 'biblio';
  if (isset($arg_list['f'])) {
    foreach ($arg_list['f'] as $l => $d) {
      if (is_array($d)) {
        foreach ($d as $v) {
          $key[] = $l;
          $key[] = $v;
        }
      }
      else {
        $key[] = $l;
        $key[] = $d;
      }
    }
  }

  if (isset($arg_list['s'])) {
    $key[] = 'sort';
    $key[] = $arg_list['s'];
  }

  if (isset($arg_list['o'])) {
    $key[] = 'order';
    $key[] = $arg_list['o'];
  }

  if (module_exists('spaces') && $space = spaces_get_space()) {
    $key[] = 'space';
    $key[] = $space->id;
  }

  return implode(':', $key);
}

function os_publications_export($format = "tagged", $nid = NULL, $popup = FALSE) {
  module_load_include('inc', 'biblio', 'includes/biblio.import.export');

  $title = 'publications';
  if ($nid !== null) {
    $node = node_load($nid);
    $title = str_replace(' ', '_', $node->title);
    $title = substr($title, 0, 8);
  }
  elseif (empty($_SESSION['last_biblio_query'])) {
    // Biblio uses this session value, if this is an anon user that has no session (or a cli user) lets set this up for them
    $args = array();
    // get default types the user has selected
    $defaults = variable_get('os_publications_default_types', array());
    if (count($defaults)) {
      $args = array('type', $defaults);
    }
    // Setup the default query
    module_load_include('inc', 'biblio', 'includes/biblio.pages');
    biblio_build_query($args);
  }

  ob_start();
  $output = biblio_export($format, $nid, $popup);
  if (empty($output)) {
    // the export didn't return anything, which means it output directly to the browser
    $output = ob_get_clean();

    // handle for spaces
    if (module_exists('spaces') && $space = spaces_get_space()) {
      $title = $space->group->purl.'-'.$title;
    }

    $header = drupal_get_http_header('Content-Disposition');

    $match = array();
    preg_match('!filename="([^"]+)"!', $header, $match);
    $filename = str_replace(array('Drupal-Biblio', 'Biblio'), $title, $match[1]);
    $header_new = preg_replace('!filename="([^"]+)"!', 'filename="'.$filename.'"', $header);

    drupal_add_http_header('Content-Disposition', $header_new);
    echo $output;
  }
  else {
    ob_end_clean();
    return $output;
  }
}