<?php

/**
 * Implementation of cite_distribute_admin()
 */
function os_mailchimp_admin() {
	$form = array();
  $form['os_mailchimp'] = array(
	  '#type' => 'fieldset',
	  '#title' => t('Configure MailChimp'),
	  '#collapsible' => FALSE,
	  '#collapsed' => FALSE,
	);

	//module mode 
  $form['os_mailchimp']['os_mailchimp_api'] = array(
	  '#type' => 'textfield',
	  '#title' => t('API Key'),
	  '#required' => TRUE,
	  '#default_value' => variable_get('os_mailchimp_api', ''), 
	  '#description' => t('Your MailChimp API Key.  Leaving this blank will disable automatic subscription'),
   );
  $form['os_mailchimp']['os_mailchimp_list'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Mailing List'),
	  '#required' => TRUE,
	  '#default_value' => variable_get('os_mailchimp_list', ''), 
	  '#description' => t('MailChimp list to subscribe new users to.'),
   );
   
   return system_settings_form($form);
}