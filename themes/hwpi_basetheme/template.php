<?php

// Rebuild the theme data.
//system_rebuild_theme_data();
//drupal_theme_rebuild();


/**
 * Preprocess variables for html.tpl.php
 */
function hwpi_basetheme_preprocess_html(&$vars) {

  $scripts = array(
    'matchMedia.js', // matchMedia polyfill for older browsers
    'eq.js',         // Equalheights script
    'eq-os.js',      // Call Equalheights for specific elements
  );
  foreach ($scripts as $script) {
    // See load.inc in AT Core, load_subtheme_script() is a wrapper for drupal_add_js()
    load_subtheme_script('js/' . $script, 'hwpi_basetheme', 'header', $weight = NULL);
  }

  // debug message
  //$message = 'Blog entry Blog Post Elements Cheat Sheet has been updated.';
  //drupal_set_message($message, 'status');
  //drupal_set_message($message, 'warning');
  //drupal_set_message($message, 'error');
}


/**
 * Preprocess variables for comment.tpl.php
 */
function hwpi_basetheme_preprocess_node(&$vars) {

  // Event nodes, inject variables for date month and day shield
  if($vars['node']->type == 'event') {
    $vars['event_start'] = array();
    if(isset($vars['field_date'][0]['value']) && !empty($vars['field_date'][0]['value'])) {
      date_default_timezone_set($vars['field_date'][0]['timezone']);
      $event_start_date = strtotime($vars['field_date'][0]['value']);
      $vars['event_start']['month'] = check_plain(date('M', $event_start_date));
      $vars['event_start']['day'] = check_plain(date('d', $event_start_date));
      $vars['classes_array'][] = 'event-start';
    }
  }

  // Event persons, change title markup to h1
  if ($vars['type'] == 'person') {
    if (isset($vars['field_person_photo'])) {
      $vars['classes_array'][] = 'with-person-photo';
    }
  }
}


/**
 * Process variables for comment.tpl.php
 */
function hwpi_basetheme_process_node(&$vars) {

  // Event persons, change title markup to h1
  if ($vars['type'] == 'person') {
    if (!$vars['teaser']) {
      $vars['title_prefix']['#suffix'] = '<h1 class="node-title">' . $vars['title'] . '</h1>';
    }
  }
}


/**
 * Alter the results of node_view().
 */
function hwpi_basetheme_node_view_alter(&$build) {

  // Persons, heavily modify the output to match the HC designs
  if ($build['#node']->type == 'person') {

    // Professional titles
    if (isset($build['field_professional_title'])) {
      $build['field_professional_title']['#label_display'] = 'hidden';
      $build['field_professional_title']['#weight'] = -10;
    }

    // Pic + Bio
    if (isset($build['field_person_photo']) || isset($build['body'])) {
      $build['pic_bio']['#prefix'] = '<div class="pic-bio clearfix">';
      $build['pic_bio']['#suffix'] = '</div>';
    }

    if (isset($build['field_person_photo'])) {
      $build['field_person_photo']['#label_display'] = 'hidden';
      $build['pic_bio']['field_person_photo'] = $build['field_person_photo'];
      unset($build['field_person_photo']);
    }

    if (isset($build['body'])) {
      $build['body']['#label_display'] = 'hidden';
      $build['pic_bio']['body'] = $build['body'];
      unset($build['body']);
    }

    // Note that Contact and Website details will print wrappers and titles regardless of any field content.
    // This is kind of deliberate to avoid having to handle the complexity of dealing with the layout or
    // setting messages etc.
    
    // Contact Details
    $build['contact_details']['#prefix'] = '<div class="block contact-details"><div class="block-inner"><h2 class="block-title">Contact Information</h2>';
    $build['contact_details']['#suffix'] = '</div></div>';

    // Contact Details > address
    if (isset($build['field_address'])) {
      $build['field_address']['#label_display'] = 'hidden';
      $build['contact_details']['field_address'] = $build['field_address'];
      unset($build['field_address']);
    }
    // Contact Details > email
    if (isset($build['field_email'])) {  
      $build['field_email']['#label_display'] = 'hidden';
      $email_plain = $build['field_email'][0]['#markup'];
      if ($email_plain) {
        $build['field_email'][0]['#markup'] = '<a href="mailto:' . $email_plain . '">email</a>';
      }
      $build['contact_details']['field_email'] = $build['field_email'];
      unset($build['field_email']);
    }
    // Contact Details > phone
    if (isset($build['field_phone'])) {
      $build['field_phone']['#label_display'] = 'hidden';
      $phone_plain = $build['field_phone'][0]['#markup'];
      if ($phone_plain) {
        $build['field_phone'][0]['#markup'] = '<em>p:</em> ' . $phone_plain;
      }
      $build['contact_details']['field_phone'] = $build['field_phone'];
      unset($build['field_phone']);
    }

    // Websites
    $build['website_details']['#prefix'] = '<div class="block website-details"><div class="block-inner"><h2 class="block-title">Websites</h2>';
    $build['website_details']['#suffix'] = '</div></div>';
    $build['field_website']['#label_display'] = 'hidden';
    $build['website_details']['field_website'] = $build['field_website'];
    unset($build['field_website']);
  }

  //dsm($build);
}


/**
 * Preprocess variables for comment.tpl.php
 */
function hwpi_basetheme_preprocess_comment(&$vars) {
  if($vars['new']) {
    $vars['title'] = $vars['title'] . '<em class="new">' . $vars['new'] . '</em>';
    $vars['new'] = '';
  }
}


/**
 * Returns HTML for a menu link and submenu.
 *
 * Adaptivetheme overrides this to insert extra classes including a depth
 * class and a menu id class. It can also wrap menu items in span elements.
 *
 * @param $vars
 *   An associative array containing:
 *   - element: Structured array data for a menu link.
 */
function hwpi_basetheme_menu_link(array $vars) {
  $element = $vars['element'];
  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  if (!empty($element['#original_link'])) {
    if (!empty($element['#original_link']['depth'])) {
      $element['#attributes']['class'][] = 'menu-depth-' . $element['#original_link']['depth'];
    }
    if (!empty($element['#original_link']['mlid'])) {
      $element['#attributes']['class'][] = 'menu-item-' . $element['#original_link']['mlid'];
    }
  }

  if (isset($element['#localized_options']) && !empty($element['#localized_options']['attributes']['title'])) {
    unset($element['#localized_options']['attributes']['title']);
  }

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>";
}
