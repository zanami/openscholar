<?php

// Rebuild the theme data. Turn this off when in production.
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
    'scripts.js',    // General stuff, currently removes ui-helper-clearfix class from ui tabs
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
    if (isset($vars['field_person_photo']) && !empty($vars['field_person_photo'])) {
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

    // Pic + Bio
    if (isset($build['field_person_photo'])) {
      $build['pic_bio']['#prefix'] = '<div class="pic-bio clearfix">';
      $build['pic_bio']['#suffix'] = '</div>';
      $build['pic_bio']['#weight'] = -9;

      if (isset($build['body'])) {
        $build['body']['#label_display'] = 'hidden';
        $build['pic_bio']['body'] = $build['body'];
        unset($build['body']);
      }
    }
    else {
      $build['body']['#weight'] = -9;
    }

    // We dont want the other fields on teasers
    if ($build['#view_mode'] == 'teaser') {
      unset($build['body'], $build['pic_bio']['body']);

      foreach (array('field_professional_title', 'field_address', 'field_email', 'field_phone', 'field_website') as $w => $f) {
        if (isset($build[$f])) {
          $build['pic_bio'][$f] = $build[$f];
          $build['pic_bio'][$f]['#weight'] = $w;
          unset($build[$f]);
        }
      }

      if (isset($build['pic_bio']['field_email'])) {
        $email_plain = $build['pic_bio']['field_email'][0]['#markup'];
        if ($email_plain) {
          $build['pic_bio']['field_email'][0]['#markup'] = '<a href="mailto:' . $email_plain . '">email</a>';
        }
      }

      if (isset($build['pic_bio']['field_phone'])) {
        $phone_plain = $build['pic_bio']['field_phone'][0]['#markup'];
        if ($phone_plain) {
          $build['pic_bio']['field_phone'][0]['#markup'] = '<em>p:</em> ' . $phone_plain;
        }
      }

      unset($build['links']['node']);

      return;
    }

    // Professional titles
    if (isset($build['field_professional_title'])) {
      $build['field_professional_title']['#label_display'] = 'hidden';
      $build['field_professional_title']['#weight'] = -10;
    }

    if (isset($build['field_person_photo'])) {
      $build['field_person_photo']['#label_display'] = 'hidden';
      $build['pic_bio']['field_person_photo'] = $build['field_person_photo'];
      unset($build['field_person_photo']);
    }

    // Note that Contact and Website details will print wrappers and titles regardless of any field content.
    // This is kind of deliberate to avoid having to handle the complexity of dealing with the layout or
    // setting messages etc.

    $block_zebra = 0;

    // Contact Details
    $build['contact_details']['#prefix'] = '<div class="block contact-details '.(($block_zebra++ % 2)?'even':'odd').'"><div class="block-inner"><h2 class="block-title">Contact Information</h2>';
    $build['contact_details']['#suffix'] = '</div></div>';
    $build['contact_details']['#weight'] = -8;

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
    $build['website_details']['#prefix'] = '<div class="block website-details '.(($block_zebra++ % 2)?'even':'odd').'"><div class="block-inner"><h2 class="block-title">Websites</h2>';
    $build['website_details']['#suffix'] = '</div></div>';
    $build['website_details']['#weight'] = -7;
    $build['field_website']['#label_display'] = 'hidden';
    $build['website_details']['field_website'] = $build['field_website'];
    unset($build['field_website']);

    if (isset($build['og_vocabulary'])) {
      foreach ($build['og_vocabulary']['#items'] as $tid) {
        $t = taxonomy_term_load($tid['target_id']);
        $v = taxonomy_vocabulary_load($t->vid);

        if (!isset($build[$v->machine_name])) {
          $m = $v->machine_name;
          $build[$m] = array(
            '#type' => 'container',
            '#weight' => $block_zebra,
            '#attributes' => array(
              'class' => array(
                'block',
                $m,
                (($block_zebra++ % 2)?'even':'odd')
              )
            ),
            'inner' => array(
              '#type' => 'container',
              '#attributes' => array(
                'class' => array('block-inner'),
              ),
              'title' => array(
                '#markup' => '<h2 class="block-title">'.$v->name.'</h2>',
              )
            ),
          );
        }

        $build[$v->machine_name]['inner'][$t->tid] = array(
          '#prefix' => '<div>',
          '#suffix' => '</div>',
          '#theme' => 'link',
          '#path' => drupal_get_path_alias('taxonomy/term/'.$t->tid),
          '#text' => $t->name,
          '#options' => array('attributes' => array(), 'html' => false),
        );
      }

      unset($build['og_vocabulary']);
    }
  }
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


/**
 * Returns HTML for a set of links.
 *
 * @param $vars
 *   An associative array containing:
 *   - links: An associative array of links to be themed. The key for each link
 *     is used as its CSS class. Each link should be itself an array, with the
 *     following elements:
 *     - title: The link text.
 *     - href: The link URL. If omitted, the 'title' is shown as a plain text
 *       item in the links list.
 *     - html: (optional) Whether or not 'title' is HTML. If set, the title
 *       will not be passed through check_plain().
 *     - attributes: (optional) Attributes for the anchor, or for the <span> tag
 *       used in its place if no 'href' is supplied. If element 'class' is
 *       included, it must be an array of one or more class names.
 *     If the 'href' element is supplied, the entire link array is passed to l()
 *     as its $options parameter.
 *   - attributes: A keyed array of attributes for the UL containing the
 *     list of links.
 *   - heading: (optional) A heading to precede the links. May be an associative
 *     array or a string. If it's an array, it can have the following elements:
 *     - text: The heading text.
 *     - level: The heading level (e.g. 'h2', 'h3').
 *     - class: (optional) An array of the CSS classes for the heading.
 *     When using a string it will be used as the text of the heading and the
 *     level will default to 'h2'. Headings should be used on navigation menus
 *     and any list of links that consistently appears on multiple pages. To
 *     make the heading invisible use the 'element-invisible' CSS class. Do not
 *     use 'display:none', which removes it from screen-readers and assistive
 *     technology. Headings allow screen-reader and keyboard only users to
 *     navigate to or skip the links. See
 *     http://juicystudio.com/article/screen-readers-display-none.php and
 *     http://www.w3.org/TR/WCAG-TECHS/H42.html for more information.
 */
function hwpi_basetheme_links($vars) {
  $links = $vars['links'];
  $attributes = $vars['attributes'];
  $heading = $vars['heading'];
  global $language_url;
  $output = '';

  if (count($links) > 0) {
    $output = '';

    if (!empty($heading)) {
      if (is_string($heading)) {
        $heading = array(
          'text' => $heading,
          'level' => 'h2',
        );
      }
      $output .= '<' . $heading['level'];
      if (!empty($heading['class'])) {
        $output .= drupal_attributes(array('class' => $heading['class']));
      }
      $output .= '>' . check_plain($heading['text']) . '</' . $heading['level'] . '>';
    }

    // Count links to use later for setting classes on the ul wrapper and on
    // each link
    $num_links = count($links);
    $i = 1;

    // Add a class telling us how many links there are, we need to check if
    // $attributes['class'] is an array because toolbar is converting this to
    // a string, if we don't check we get a fatal error. This class is added
    // to aid in potential cross browser issues with the full width ui.tabs
    if (isset($attributes['class']) && is_array($attributes['class'])) {
      $attributes['class'][] = 'num-links-' . $num_links;
    }
    $output .= '<ul' . drupal_attributes($attributes) . '>';

    foreach ($links as $key => $link) {
      // Add classes to make theming the ui.tabs much easier/possible
      $class = array();
      $class[] = 'link-count-' . $key;
      if ($i == 1) {
        $class[] = 'first';
      }
      if ($i == $num_links) {
        $class[] = 'last';
      }
      if (!empty($class)) {
        $output .= '<li' . drupal_attributes(array('class' => $class)) . '>';
      }
      else {
        $output .= '<li>';
      }
      if (isset($link['href'])) {
        $output .= l($link['title'], $link['href'], $link);
      }
      elseif (!empty($link['title'])) {
        if (empty($link['html'])) {
          $link['title'] = check_plain($link['title']);
        }
        $span_attributes = '';
        if (isset($link['attributes'])) {
          $span_attributes = drupal_attributes($link['attributes']);
        }
        $output .= '<span' . $span_attributes . '>' . $link['title'] . '</span>';
      }

      $i++;
      $output .= "</li>";
    }

    $output .= '</ul>';
  }

  return $output;
}

/**
 * Returns HTML for status and/or error messages, grouped by type.
 *
 * Adaptivetheme adds a div wrapper with CSS id.
 *
 * An invisible heading identifies the messages for assistive technology.
 * Sighted users see a colored box. See http://www.w3.org/TR/WCAG-TECHS/H69.html
 * for info.
 *
 * @param $vars
 *   An associative array containing:
 *   - display: (optional) Set to 'status' or 'error' to display only messages
 *     of that type.
 */
function hwpi_basetheme_status_messages($vars) {
  $display = $vars['display'];
  $output = '';

  $status_heading = array(
    'status' => t('Status update'),
    'error' => t('Error'),
    'warning' => t('Warning'),
  );
  foreach (drupal_get_messages($display) as $type => $messages) {
    $output .= '<div class="messages ' . $type . '"><div class="message-inner"><div class="message-wrapper">';
    if (!empty($status_heading[$type])) {
      $output .= '<h2>' . $status_heading[$type] . "</h2>";
    }
    if (count($messages) > 1) {
      $output .= " <ul>";
      foreach ($messages as $message) {
        $output .= '  <li>' . $message . "</li>";
      }
      $output .= " </ul>";
    }
    else {
      $output .= $messages[0];
    }
    $output .= "</div></div></div>";
  }
  return $output;
}

/**
 * Implements hook_theme_registry_alter
 * 
 * Changes the theme template for os_slideshow's controls.
 */
function hwpi_basetheme_theme_registry_alter(&$reg) {
  $reg['views_slideshow_controls_text']['template'] = drupal_get_path('theme', 'hwpi_basetheme') . '/views-view--os_slideshow-slideshow-controls-text';
}

/**
 * Implements hook_preprocess_HOOK
 * 
 * Renders a slideshow pager and adds it as a var for our slideshow-controls template.
 */
function hwpi_basetheme_preprocess_views_slideshow_controls_text(&$vars) {
  $settings = array(
    'enable' => TRUE,
    'weight' => 1,
    'hide_on_single_slide' => 0,
    'type' => 'views_slideshow_pager_fields',
    'views_slideshow_pager_numbered_hover' => 0,
    'views_slideshow_pager_numbered_click_to_page' => 0,
    'views_slideshow_pager_thumbnails_hover' => 0,
    'views_slideshow_pager_thumbnails_click_to_page' => 0,
    'views_slideshow_pager_fields_fields' => array('title' => 0),
    'views_slideshow_pager_fields_hover' => 0,
  );
  
  $vars['rendered_pager'] = theme('views_slideshow_pager_widget_render', array(
    'vss_id' => $vars['vss_id'], 
    'view' => $vars['view'], 
    'settings' => $settings, 
    'location' => $vars['location'], 
    'rows' => $vars['rows']
  ));
}
 
