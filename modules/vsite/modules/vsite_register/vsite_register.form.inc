<?php

/**
 * Form for creating new VSites.
 */
function vsite_register_form($form, &$form_state) {
  global $base_url;
  global $user;
  $form = array();
  $manager = user_access('create vsite content on behalf') && user_access('administer users');

  // @todo: What is the parent?
  $parent = NULL;
  if (isset($_GET['parent']) && $parent = vsite_get_vsite($_GET['parent'])) {
    $parent = $_GET['parent'];
  }
  $form['parent'] = array(
    '#type' => 'hidden',
    '#value' => $parent,
  );

  $form['domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Your URL'),
    '#required' => TRUE,
    '#field_prefix' => $base_url . '/',
    '#size' => 35,
    '#description' => t('Minimum of 3 characters, which must be lowercase. Punctuation is not allowed except for hyphens and underscores. <br />Example: a choice of \'jdoe\' would result in the URL: %site.', array('%site' => $base_url . '/jdoe')),
    '#element_validate' => array('vsite_register_validate_domain'),
    '#ajax' => array(
      'callback' => 'vsite_register_ajax_domain',
      'wrapper' => 'wrapper-domain',
    ),
    // Don't limit validation erros, as we want the validation to happen also
    // on the Ajax request.
    '#limit_validation_errors' => FALSE,
    '#prefix' => '<div id="wrapper-domain">',
    '#suffix' => '</div>',

    // Use the theme function to add the div with the correct wrapper ID.
    '#field_suffix' => theme('vsite_register_form_errors', array('name' => 'domain')),
    // @todo: Re-visit JS.
    // '#attached' => array('js' => array(drupal_get_path('module', 'vsite_register') . '/vsite_register.js')),
  );

  $group_types = og_get_all_group_bundle();
  $bundles = array();
  foreach ($group_types['node'] as $bundle => $label) {
    if (user_access("create {$bundle} content") && (!isset($parent) || in_array($bundle, array('personal', 'project')))) {
      $bundles[$bundle] = $label;
    }
  }
  $default_bundle = variable_get('os_default_group_bundle', 'personal');
  if(!isset($bundles[$default_bundle])){
    $default_bundle = current($bundles);
  }

  $form['bundle'] = array(
    '#title' => t('Type of site'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $bundles,
    '#default_value' => $default_bundle,
  );

  if (count($bundles) == 1) {
    $form['bundle']['#type'] = 'value';
    $form['bundle']['#value'] = $default_bundle;
  }
  else{
    $form['bundle']['#ajax'] = array(
      'event' => 'change',
      'wrapper' => 'preset-wrapper',
      'callback' => 'vsite_register_ajax_available_presets',
      'method' => 'replace',
    );
  }

  $selected_bundle = isset($form_state['values']['bundle']) ? $form_state['values']['bundle'] : $default_bundle;

  //Get enabled presets and load their titles.
  $enabled_presets = variable_get('os_enabled_spaces_presets', array());
  $presets = spaces_preset_load();
  foreach($enabled_presets as $key => $value) {
    $enabled_presets[$key] = $presets[$key]->title;
    //Remove presets that don't go with this bundle
    if(isset($presets[$key]->value['bundles']) && !in_array($selected_bundle, $presets[$key]->value['bundles'])){
     unset($enabled_presets[$key]);
    }
  }
  $default_preset = variable_get('spaces_preset_og', 'os_scholar');

  $form['preset'] = array(
    '#prefix' => '<div id="preset-wrapper">',
    '#suffix' => '</div>',
    '#title' => t('Initial Setup'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $enabled_presets,
    '#default_value' => (isset($enabled_presets[$default_preset])) ? $default_preset : current(array_keys($enabled_presets)),
  );

  if (count($enabled_presets) == 1) {
    $form['preset']['#type'] = 'value';
    $form['preset']['#value'] = $form['preset']['#default_value'];
  }

  $form['vsite_private'] = array(
    '#title' => t('Private Site'),
    '#type' => 'checkbox',
    '#default_value' => FALSE,
    '#description' => t('Make your site private.  It won\'t be visible to anyone but you and won\'t be indexed by Google until you make it public.')
  );


  if (!$user->uid) {
    //anonymous users always create new accounts
    $form += _vsite_register_user_form();
    $form['vicarious_user'] = array(
      '#type' => 'hidden',
    	'#value' => FALSE,
    );
    //flag for JS to set when create new user is clicked
    $form['user_fieldset']['create_new_user'] = array(
      '#type' => 'hidden',
      '#default_value' => TRUE,
    );
  }
  elseif($manager) {
    // User can create a new site, give a site to another user, or give a site to a new account.
    $form['vicarious_user'] = array(
      '#type' => 'checkbox',
    	'#title' => t('Create this OS Site on behalf of another user.'),
    );

    $form['user_fieldset'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
    	'#states' => array('visible' => array(':input[name="vicarious_user"]' => array('checked' => TRUE))),
    );

    $ajax_link = '<a href="javascript:void(0)" id="new-user-link">' . t('create a new user') . '</a>';

    $form['user_fieldset']['existing_username'] = array(
      '#type' => 'textfield',
    	'#title' => t('Find Existing User'),
      '#description' => t('Enter the name of the user who will manage this new site or !create_new.',
        array('!create_new' => $ajax_link)
      ),
    	'#autocomplete_path' => 'user/autocomplete',
    );

    // Flag for JS to set when create new user is clicked.
    $form['user_fieldset']['create_new_user'] = array(
      '#type' => 'hidden',
      '#default_value' => FALSE,
    );

    $form['user_fieldset']['new_user_div'] = array(
      '#markup' => '<div id="new_user_div">',
    );

    $form['user_fieldset'] += _vsite_register_user_form();

    $form['user_fieldset']['huid'] = array();

    $form['user_fieldset']['new_user_close_div'] = array(
      '#markup' => '</div>',
    );

  }
  else {
    // Regular users can only create sites for themselves.
    $form['vicarious_user'] = array(
      '#type' => 'value',
      '#value' => FALSE,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create your site'),
    '#weight' => 40,
    '#prefix' => '<div id="form-errors"></div>',
    '#suffix' => '<div id="submit-suffix"></div>',
    '#submit' => array('vsite_register_form_submit'),
  );

  return $form;

}

/**
 * Form builder; Returns the "user" section of vsite-register form.
 */
function _vsite_register_user_form() {
  $form = array();
  $form['name'] = array(
    '#type' => 'textfield',
    '#size' => 35,
    '#title' => t('Desired user name'),
    '#description' => t('Spaces are allowed; punctuation is not allowed except for periods, hyphens, and underscores.'),
    '#ajax' => array(
      // @todo: Re-add callback.
      // 'callback' => 'vsite_register_ajax_validation',
      'wrapper' => 'name-errors',
    ),
    '#field_suffix' => theme('vsite_register_form_errors', array('name'=>'name')),
  );

  $form['first_name'] = array(
    '#type' => 'textfield',
    '#size' => 35,
    '#title' => t('First name'),
  );

  $form['last_name'] = array(
    '#type' => 'textfield',
    '#size' => 35,
    '#title' => t('Last name'),
  );

  $form['mail'] = array(
    '#type' => 'textfield',
    '#size' => 35,
    '#title' => t('Email address'),
    '#description' => t('A valid e-mail address is required. All e-mails from OpenScholar will be sent to this address.'),
    '#ajax' => array(
      // @todo: Re-add callback.
      // 'callback' => 'vsite_register_ajax_validation',
      'wrapper' => 'mail-errors',
    ),
    '#field_suffix' => theme('vsite_register_form_errors', array('name'=>'mail')),
  );

  // Password field.
  $form['password'] = array(
    '#type' => 'password_confirm',
    '#size' => 35,
  );

  return $form;
}

/**
 * Validate handler; Validate the "domain" form element.
 */
function vsite_register_validate_domain($element, &$form_state) {
  $values = $form_state['values'];
  if (empty($values['domain'])) {
    return;
  }

  if (strlen($values['domain']) < 3 || !valid_url($values['domain']) || !_vsite_register_valid_url($values['domain'])) {
    form_error($element ,t('Invalid URL specified.'));
  }

  if (($purl = purl_load(array('value'=>$values['domain'], 'provider'=>'spaces_og'), TRUE)) || menu_get_item($values['domain'])) {
    form_error($element ,t('URL %url is taken.  Please choose another.', array('%url' => $values['domain'])));
  }
}

/**
 * Ajax callback; Return the domain form element, with any error if exists.
 */
function vsite_register_ajax_domain($form, &$form_state) {
  $element_name = 'domain';
  if ($error = form_get_error($form[$element_name])) {
    $form[$element_name]['#field_suffix'] = theme('vsite_register_form_errors', array(
      'name' => $element_name,
      'errors' => array($error),
    ));
  }

  return $form[$element_name];
}


/**
 * Submit handler; Creates user and vsite when site/register form is submitted
 */
function vsite_register_form_submit(&$form, &$form_state) {
  ctools_include('user', 'os');
  ctools_include('vsite', 'vsite');
  $values = $form_state['values'];

  if ($values['vicarious_user'] && $values['existing_username']) {
    //user account already exists
    $site_owner = user_load_by_name($values['existing_username']);

  }
  elseif (($values['vicarious_user'] && !$values['existing_username']) || (!$values['vicarious_user'] && $values['name'])) {
    // create user for self or on someone's behalf
    $user_options = array(
    	'name' => $values['name'],
    	'pass' => $values['password'],
    	'mail' => $values['mail'],
    	'status' => 1,
      'field_first_name' => $values['first_name'],
      'field_last_name' => $values['last_name'],
    );
    $site_owner = os_user_create($user_options);

    // Log in as new user, if we're not already logged in.
    global $user;
    if ($user->uid == 0) {
      $user = $site_owner;
      user_login_finalize($form_state);
    }

  }
  else {
    //creating site for self.  no need to create a new user
    global $user;
    $site_owner = $user;
  }

  //create the vsite
  $vsite = vsite_create_vsite($values['domain'], $values['domain'], $site_owner->uid, $values['bundle'], $values['preset'], $_POST['parent']);
  //add user to vsite
  module_load_include('inc', 'vsite', 'includes/user');
  vsite_user_add($site_owner);

  os_role_grant($site_owner->uid, 'vsite admin', $vsite->id);

  if (isset($values['vsite_private']) && $values['vsite_private'] && module_exists('vsite_access')) {
    $node = node_load($vsite->group->nid);
    $node->group_access[LANGUAGE_NONE][0]['value'] = 1;
    node_save($node);
  }

  // Email user to inform them of new created site, email is always sent.
  vsite_register_notify_user($vsite);

  drupal_set_message(vsite_register_get_message($values['domain']));
}

/**
 * Get the confirmation message.
 *
 * @todo: Redirect to new vsite.
 */
function vsite_register_get_message($domain) {
  global $user;

  $message  = '<div id="success-message" class="status">';
  if (user_access("create openscholar site on behalf of others")) {
    $message .= '<p>' . t('Success! the new site has been created.') . '</p>';
  }
  else {
    $message .= '<p>' . t('Success! your new site has been created.') . '</p>';
  }

  if (!$user->uid) {
    $message .= '<p>' . t('Check your email account for further details on how to login in to start adding features and content.') . '</p>';
  }

  $params = array('!url' => url($domain, array('absolute' => TRUE)));
  $message .= '<p>' . t('The URL of the site is !url. <a href="!url">Go there now</a>', $params) . '</p>';
  $message .= '</div>';

  return $message;
}


/**
 * Validation function for site/register form.
 */
function vsite_register_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];

  if ($values['create_new_user']) {
    //tell javascript to switch to new user form instead of making user switch it again.
    drupal_add_js(array('vsite_register' => array('new_user_clicked' => TRUE)), 'setting');
  }
}

/**
 * Update the available presets.
 */
function vsite_register_ajax_available_presets($form, $form_state) {
  //Hard work is done in the form build function, just return the item
  return $form['preset'];
}

/**
 * Return TRUE if a domain name is valid for a vsite.
 *
 */
function _vsite_register_valid_url($url) {
  foreach (array('!', ';', '/', '?', ':', '@', '&', '=', '"', "'",) as $char) {
    if (strpos($url, $char) !== FALSE) {
      return FALSE;
    }
  }

  return purl_validate(array('value' => $url, 'provider' => 'spaces_og'));
}

/*
 * Function to send out an email to the user
 * to notify them of their newly created site.
 */
function vsite_register_notify_user($vsite){
  global $language;
  global $base_url;
  global $user;

  // Check to see if the user logged in matched the newly created user.
  if($user->uid == $vsite->group->uid) {
  	$need_pass_reset = FALSE;
  }else {
  	$need_pass_reset = TRUE;
  }

  $params = array();
  $from = variable_get('site_mail', ini_get('sendmail_from'));
  $new_user = user_load($vsite->group->uid);

  //add vsite and user object
  $params['vsite'] = $vsite;
  $params['user'] = $new_user;

  $scholar_site_url = url($base_url.'/'.$vsite -> group->title, $options = array('absolute' => TRUE));
  $count = count(vsite_get_vsite_by_owner($new_user -> uid));
  $timestamp = time();

  $subject = t("An OpenScholar web site has been created for you.", array(
    '!site_name' => variable_get('site_name', 'OpenScholar')
  ));
  $msg_variables = array(
    '!vsite-count' => $count,
    '!username' => $new_user->name,
    '!site' => variable_get('site_name', 'OpenScholar'),
    '!scholar_site_url' => $scholar_site_url,
    '!login_url' => user_pass_reset_url($new_user),
    '!site_login_uri' => url($base_url.'/'.$vsite -> group->title . '/home', array('absolute' => TRUE)),
    '!uri' => $base_url,
    '!uri_brief' => preg_replace('!^https?://!', '', $base_url),
    '!mailto' => $new_user->mail,
    '!date' => format_date(time(), 'medium', '', NULL, $message['language']->language),
    '!login_uri' => url('user', array(
      'absolute' => TRUE,
      'language' => $message['language']
    )),
    '!site_url' => url($vsite -> group -> purl),
    '!edit_uri' => url('user/' . $new_user->uid . '/edit', array(
      'absolute' => TRUE,
      'language' => $message['language']
    )),
    '!scholar_edit_url' => url("/user/{$new_user->uid}/edit", array('absolute' => TRUE)),
  );

  //detemine if user is creating first site or additional site, send appropriate message
  if(count(vsite_get_vsite_by_owner($new_user -> uid)) > 1 || $need_pass_reset == FALSE){
    $body = t("Hi !username,\n\nYour new web site at !site has been activated. Your web site url is !scholar_site_url\n\nYou will be able to log into your site at !site_login_uri with username:\n\n!username\n\nWe hope you will enjoy your new web site.\n\nSincerely,\nThe !site Team",
     $msg_variables);
  }else{
    $body = t("Hi !username,\n\nYour new web site at !site has been activated.\n\nYour web site url is !scholar_site_url\n\nYou may now log in and reset your password by clicking on this link or copying and pasting it in your browser: \n\n !login_url \n\nThis login can be used only once. After logging in, you will be redirected to !scholar_edit_url so you can change your password. Once you have set your own password, you will be able to log in at !site_login_uri as username:\n\n!username\n\nWe hope you will enjoy your new web site.\n\nSincerely,\nThe !site Team",
    $msg_variables);
  }

  $params['subject'] = $subject;
  $params['body'] = $body;
  $result = drupal_mail('vsite_register', 'vsite_register_notify', $new_user->mail, $language, $params, $from, $send=TRUE);
  if($result == FALSE) {
  	drupal_set_message(t('Error sending notification mail to user.'), 'error');
  }
}

/**
 * Implements hook_mail().
 */
function vsite_register_mail($key, &$message, $params) {
  $message['subject'] = $params['subject'];
  $message['body'][] = $params['body'];

  //these values be used with any mail alter hooks
  $message['vsite'] = $params['vsite'];
  $message['user'] = $params['user'];
}
