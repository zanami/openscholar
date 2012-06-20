<?php 

/**
 * @function vsite_site_register_form
 *
 * Provides form for site/register
 */
function vsite_site_register_form($form, &$state) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#size' => 35,
    '#title' => t('Desired user name'),
    '#required' => TRUE,
    '#description' => t('Spaces are allowed; punctuation is not allowed except for periods, hyphens, and underscores.'),
  );

  // last name
  $form['mail'] = array(
    '#type' => 'textfield',
    '#size' => 35,
    '#title' => t('Your email address'),
    '#required' => TRUE,
    '#description' => t('A valid e-mail address is required. All e-mails from OpenScholar will be sent to this address.'),
  );
  
  // password field
  $form['password'] = array(
    '#type' => 'password_confirm',
    '#size' => 35,
    '#required' => TRUE,
    '#description' => ''
  );
  
  //HUID?
  //or is that added by another module?
  
  //preset, ignoring the og ones
  //  make a var to hold presets so users can choose personal/dept.  if only one avail, don't ask
  
  global $base_url;
  $form['domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Your URL'),
    '#required' => TRUE,
    '#field_prefix' => $base_url . '/',
    '#size' => 35,
    '#weight' => 20,
    '#description' => t('Minimum of 3 characters. Punctuation is not allowed except for hyphens and underscores. <br />Example: a choice of \'jdoe\' would result in the URL: %site.', array(
      '%site' => $base_url . '/jdoe'
    )),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create your site'),
    '#weight' => 40,
  );

  return $form;
  
}

/**
 * @function vsite_site_register_form
 *
 * Creates user and vsite when site/register form is submitted
 */
function vsite_site_register_form_submit(&$form, &$state) {
  ctools_include('user', 'os');
  ctools_include('vsite', 'vsite');
  $values = $state['values'];
  
  // create user
  $user_options = array('name' => $values['name'],'pass' => $values['password'],'mail' => $values['mail'],'status' => 1,);
  $user = os_user_create($user_options, FALSE);
  
  // create vsite - os_vsite_create
  $preset = (isset($values['preset'])) ? $values['preset'] : variable_get('os_preset_type', 'vsite');
  vsite_create_vsite($values['name'], $values['domain'], $user->uid, $preset);
  
  if ($vsite = spaces_load('og', $node->nid)) {
    //notify user
    //add user to vsite
    os_user_add($user->uid, $vsite->id);
    os_role_grant($user->uid, 'vsite admin', $vsite->id);
  } else {
    //errors
  }
  

}


/**
 * @function vsite_site_register_form_validate
 *
 * Validation function for site/register form
 */
function vsite_site_register_form_validate(&$form, &$state) {
  $values = $state['values'];
  
  //mail
  if ($mail_error = user_validate_mail($values['mail'])) {
    form_set_error('mail', $mail_error);
  }
  
  if (_vsite_mail_in_use($values['mail'])) {
    form_set_error('mail', t('Email address already in use.  Please try another.'));
  }
  
  //username
  if ($user_error = user_validate_name($values['name'])) {
    form_set_error('name', $user_error);
  }
  
  if ($user = user_load_by_name($values['name'])) {
    form_set_error('name', t('Username %name is taken.  Please choose another.', array('%name' => $values['name'])));
  }
  
  //domain
  if (!purl_validate($values['domain']) || strlen($values['domain']) < 3) {
    form_set_error('domain', t('Invalid URL specified.'));
  }
  
  if (purl_load($values['domain']) || menu_get_item($values['domain'])) {
    form_set_error('domain', t('URL %url is taken.  Please choose another.', array('%url'=>$values['domain'])));
  }
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
function _vsite_mail_in_use($mail) {
  return (bool) db_select('users')
              ->fields('users', array('uid'))
              ->condition('uid', $account->uid, '<>')
              ->condition('mail', db_like($mail, 'LIKE'))
              ->range(0, 1)
              ->execute()
              ->fetchField();
}