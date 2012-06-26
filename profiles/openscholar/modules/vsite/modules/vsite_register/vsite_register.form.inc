<?php 

/**
 * @function vsite_site_register_form
 *
 * Provides form for site/register
 */
function vsite_register_form($form, &$state) {    
  global $base_url;
  global $user;
  $form = array();
  $anonymous = ($user->uid == 0);
  $manager = (user_access('create vsite content on behalf') && user_access('administer users'));
  
  $form['domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Your URL'),
    '#required' => TRUE,
    '#field_prefix' => $base_url . '/',
    '#size' => 35,
    '#description' => t('Minimum of 3 characters. Punctuation is not allowed except for hyphens and underscores. <br />Example: a choice of \'jdoe\' would result in the URL: %site.', array(
      '%site' => $base_url . '/jdoe'
    )),
  );
  
  //Get enabled presets and load their titles
  $enabled_presets = variable_get('os_enabled_spaces_presets', array());
  $presets = spaces_preset_load();
  foreach($enabled_presets as $key => $value) {
    $enabled_presets[$key] = $presets[$key]->title;
  }
  $default_preset = variable_get('spaces_preset_og', 'os_personal');
  
  $form['preset'] = array(
    '#title' => t('Type of site'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $enabled_presets,
    '#default_value' => (isset($enabled_presets[$default_preset])) ? $default_preset : current($enabled_presets),
  );
  
  if (count($enabled_presets) <= 1) {
    $form['preset']['#type'] = 'value';
    $form['preset']['#value'] = current($enabled_presets);
  }
    
  
  if ($anonymous) {
    //anonymous users always create new accounts
    $form += _vsite_register_user_form();
    
  } elseif($manager) {
    //managers can create a new site, give a site to another user, or give a site to a new account.
    $form['vicarious_user'] = array(
      '#type' => 'checkbox',
    	'#title' => t('Create this OS Site on behalf of another user.'),
    );
    
    $form['user_fieldset'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
    	'#states' => array('visible' => array(':input[name="vicarious_user"]' => array('checked' => TRUE))),
      '#attached' => array('js' => array(drupal_get_path('module', 'vsite_register') . '/vsite_register.js')),
    );
    
    //$ajax_link = l(t('create a new user'), 'javascript:void(0)', array('attributes'=>array('id'=>array('new-user-link'))));
    $ajax_link = '<a href="javascript:void(0)" id="new-user-link">' . t('create a new user') . '</a>'; //figure out l() later
    
    $form['user_fieldset']['existing_username'] = array(
      '#type' => 'textfield',
    	'#title' => t('Find Existing User'),
      '#description' => t('Enter the name of the user who will manage this new site or !create_new.',
        array('!create_new' => $ajax_link) 
      ),
    	'#autocomplete_path' => 'user/autocomplete',
    );
    
    $form['user_fieldset']['new_user_div'] = array(
      '#markup' => '<div id="new_user_div">',
    );
    
    $form['user_fieldset'] += _vsite_register_user_form();
    
    $form['user_fieldset']['new_user_close_div'] = array(
      '#markup' => '</div>',
    );

  } else {
    //regular users can only create sites for themselves.
    $form['vicarious_user'] = array(
      '#type' => 'value',
      '#value' => FALSE,
    );
  }
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create your site'),
    '#weight' => 40,
  );

  return $form;
  
}

//returns user section of vsite_register form
function _vsite_register_user_form() {
  $form = array();
  $form['name'] = array(
    '#type' => 'textfield',
    '#size' => 35,
    '#title' => t('Desired user name'),
    '#description' => t('Spaces are allowed; punctuation is not allowed except for periods, hyphens, and underscores.'),
  );

  $form['mail'] = array(
    '#type' => 'textfield',
    '#size' => 35,
    '#title' => t('Email address'),
    '#description' => t('A valid e-mail address is required. All e-mails from OpenScholar will be sent to this address.'),
  );
  
  // password field
  $form['password'] = array(
    '#type' => 'password_confirm',
    '#size' => 35,
    '#description' => t('Password'),
  );
  
  $form['notify'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send email notification of this account.'),
  );
  
  return $form;
}



/**
 * @function vsite_site_register_form
 *
 * Creates user and vsite when site/register form is submitted
 */
function vsite_register_form_submit(&$form, &$state) {
  ctools_include('user', 'os');
  ctools_include('vsite', 'vsite');
  $values = $state['values'];
  
  if ($values['vicarious_user'] && $values['existing_username']) {
    $user = user_load_by_name($values['existing_username']);
  } elseif ($values['vicarious_user'] && !$values['existing_username']) {
    // create user
    $user_options = array(
    	'name' => $values['name'],
    	'pass' => $values['password'],
    	'mail' => $values['mail'],
    	'status' => 1,
    );
    $user = os_user_create($user_options, FALSE);
    
    //and send some email
    if ($values['notify']) {
      _user_mail_notify('register_no_approval_required', $user);
    }
  } else {
    global $user;
  }
  
  //create the vsite
  if ($vsite = vsite_create_vsite($user->name, $values['domain'], $user->uid, $values['preset'])) {
    //add user to vsite
    os_user_add($user->uid, $vsite->id);
    os_role_grant($user->uid, 'vsite admin', $vsite->id);
       
    drupal_set_message(t('Welcome to your new OpenScholar site.'));
    drupal_goto($values['domain']);
  } else {
    //errors
  }
  
}


/**
 * @function vsite_site_register_form_validate
 *
 * Validation function for site/register form
 */
function vsite_register_form_validate(&$form, &$state) {
  $values = $state['values'];
  
  if ($values['vicarious_user'] && $values['existing_username']) {
    
    //Giving a site to existing user
    if (!$user = user_load_by_name($values['existing_username'])) {
      form_set_error('existing_username', t('Username does not exist.  Did you mean to createa new user instead?'));
    }
    
  } elseif ($values['vicarious_user'] && !$values['existing_username']) {
    
    //Giving a site to a new user
    
    //mail
    if ($mail_error = user_validate_mail($values['mail'])) {
      form_set_error('mail', $mail_error);
    }
    
    if (_vsite_register_mail_in_use($values['mail'])) {
      form_set_error('mail', t('Email address already in use.  Please try another.'));
    }
    
    //username
    if ($user_error = user_validate_name($values['name'])) {
      form_set_error('name', $user_error);
    }
    
    if ($user = user_load_by_name($values['name'])) {
      form_set_error('name', t('Username %name is taken.  Please choose another.', array('%name' => $values['name'])));
    }
    
    //password 
    if (!$values['password']) {
      form_set_error('password', t('You must enter a password'));
    }
  
    //tell javascript to switch to new user form instead of making user switch it again.
    drupal_add_js(array('vsite_register' => array('new_user_clicked' => TRUE)), 'setting');
  }
  
  //domain availability applies to all cases
  
  //domain
  if (!purl_validate($values['domain']) || strlen($values['domain']) < 3) {
    form_set_error('domain', t('Invalid URL specified.'));
  }
  
  if (purl_load(array('value'=>$values['domain'], 'provider'=>'spaces_og')) || menu_get_item($values['domain'])) {
    form_set_error('domain', t('URL %url is taken.  Please choose another.', array('%url'=>$values['domain'])));
  }
  
  //check that user can make more vsites if making for self
}

/**
 * @function _vsite_mail_in_use
 * 
 * Checks if an email address is already in use by another user.
 * Code borrowed from user.module's user_account_form_validate()
 * 
 * @param string $mail
 * 	email address
 */
function _vsite_register_mail_in_use($mail) {
  return (bool) db_select('users')
              ->fields('users', array('uid'))
              ->condition('mail', db_like($mail, 'LIKE'))
              ->range(0, 1)
              ->execute()
              ->fetchField();
}