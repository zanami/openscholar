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
  
  $form['preset'] = array(
    '#title' => t('Type of site'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $enabled_presets,
    '#default_value' => current($enabled_presets),
  );
  
  if (count($enabled_presets) <= 1) {
    $form['preset']['#type'] = 'value';
    $form['preset']['#value'] = current($enabled_presets);
  }
    
  
  if ($anonymous) {
    $form += _vsite_register_user_form();
    
  } elseif($manager) {
    $form['new_user'] = array(
      '#type' => 'checkbox',
    	'#title' => t('Create new user account to manage this OS Site.'),
    );
  
    $form['user_fieldset'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
    	'#states' => array('visible' => array(':input[name="new_user"]' => array('checked' => TRUE))),
    );
    
    $form['user_fieldset'] += _vsite_register_user_form();
  } else {
    
    $form['new_user'] = array(
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
    '#title' => t('Your email address'),
    '#description' => t('A valid e-mail address is required. All e-mails from OpenScholar will be sent to this address.'),
  );
  
  // password field
  $form['password'] = array(
    '#type' => 'password_confirm',
    '#size' => 35,
    '#description' => '',
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
  
  if ($values['new_user']) {
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
  
  if ($values['new_user']) {
    //mail
    if ($mail_error = user_validate_mail($values['mail'])) {
      form_set_error('mail', $mail_error);
    }
    
    if (_vsite_register_mail_in_use($values['mail'])) {
      form_set_error('mail', t('Email address already in use.  Please try another.'));
    }
    
    if ($user_error = user_validate_name($values['name'])) {
      form_set_error('name', $user_error);
    }
    
    //username
    if ($user = user_load_by_name($values['name'])) {
      form_set_error('name', t('Username %name is taken.  Please choose another.', array('%name' => $values['name'])));
    }
  } else {
    //clear potential password errors.  browsers can fill in a pw even if it's not selected, making it fail validation
    $errors = form_get_errors();
    if ($msg = $errors['password']) {
      $_SESSION['messages']['error'] = array_diff($_SESSION['messages']['error'], array($msg));
      if (empty($_SESSION['messages']['error'])) {
        unset($_SESSION['messages']['error']); 
      }
    }
  }
  
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