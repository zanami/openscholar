<?php

/**
 * Implements hook_install_tasks().
 */
function openscholar_install_tasks($install_state) {
  $tasks = array();
  
  // OS flavors (production, development, etc)
  $tasks['openscholar_flavor_form'] = array(
    'display_name' => t('Choose a flavor'),
    'type' => 'form'
  );
  
  // Simple form to select the installation type (single site or multitenant)
  $tasks['openscholar_install_type'] = array(
    'display_name' => t('Installation type'),
    'type' => 'form'
  );
  
  // If multitenant, we need to do some extra work, e.g. some extra modules
  // otherwise, skip this step
  $tasks['openscholar_vsite_modules_batch'] = array(
    'display_name' => t('Choose a flavor'),
    'type' => 'batch',
    'run' => (variable_get('os_profile_type', '') == 'vsite') ? INSTALL_TASK_RUN_IF_NOT_COMPLETED : INSTALL_TASK_SKIP
  );
  
  return $tasks;
}

/**
 * Flavor selection form
 */
function openscholar_flavor_form($form, &$form_state) {
  
  $options = array(
    'production' => 'Production Deployment',
    'development' => 'Development',
  );
  
  $form['os_profile_flavor'] = array(
    '#title' => t('Select a flavor'),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => 'personal'
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Next'
  );
  
  return $form;
}


/**
 * Install type selection form
 */
function openscholar_install_type($form, &$form_state) {

  $options = array('novsite' => 'Single site install', 'vsite' => 'Multi-tenant install');

  $form['os_profile_type'] = array(
    '#title' => t('Installation type'),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => 'vsite',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}


/**
 * Form submit handler when selecting an installation type
 */
function openscholar_flavor_form_submit($form, &$form_state) {
  $flavor = $form_state['input']['os_profile_flavor'];
  switch($flavor){
    case 'production':
      //@todo when production
      break;
    case 'development':       // dev
      //@todo when development
      //os_add_permissions(1, array('switch users')); //  yes anon users can switch users!!
      break;
  }
  
  variable_set('os_profile_flavor', $flavor);
}


/**
 * Form submit handler when selecting an installation type
 */
function openscholar_install_type_submit($form, &$form_state) {
  if ($form_state['input']['os_profile_type'] == 'vsite'){
      variable_set('os_profile_type', 'vsite');
  }
  else {
      // nothing for now
  }
}



function openscholar_vsite_modules_batch(&$install_state){
  //@todo this should be in an .inc file or something.
  $modules = array(
    'entityreference',
    'purl',
    'spaces',
    'spaces_og',
    'og_views',
    'vsite',
    'vsite_domain',
    'vsite_menu',
    'vsite_layout',
    'vsite_register',
    'vsite_access',
    'og'
  );
  
  return _opnescholar_module_batch($modules);
}

/**
 * Returns a batch operation definition that will install some $modules
 *
 * @param $modules
 *   An array of names of modules to install
 *
 * $return
 *   A batch definition.
 *
 * @see
 *   http://api.drupal.org/api/drupal/includes%21install.core.inc/function/install_profile_modules/7
 */
function _opnescholar_module_batch($modules) {
  $t = get_t();
  
  $files = system_rebuild_module_data();
  
  // Always install required modules first. Respect the dependencies between
  // the modules.
  $required = array();
  $non_required = array();
  
  // Add modules that other modules depend on.
  foreach ( $modules as $key => $module ) {
    if (isset($files[$module]) && $files[$module]->requires) {
      $modules = array_merge($modules, array_keys($files[$module]->requires));
    }
  }
  $modules = array_unique($modules);
  foreach ( $modules as $module ) {
    if (! empty($files[$module]->info['required'])) {
      $required[$module] = $files[$module]->sort;
    }
    else {
      $non_required[$module] = $files[$module]->sort;
    }
  }
  arsort($required);
  arsort($non_required);
  
  $operations = array();
  foreach ( $required + $non_required as $module => $weight ) {
    if (isset($files[$module])) {
      $operations[] = array('_install_module_batch',
        array(
          $module,
          $files[$module]->info['name']
        )
      );
    }
  }
  $batch = array(
    'operations' => $operations,
    'title' => st('Installing @current out of @total modules.'),
    'error_message' => st('The installation has encountered an error.'),
    //'file' => 'includes/install.core.inc',
    'finished' => '_install_profile_modules_finished'
  );
  return $batch;
}

/**
 * Implements hook_form_FORM_ID_alter().
 **/
function openscholar_form_install_configure_form_alter(&$form, $form_state) {
  // Pre-populate the site name with the server name.
  $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
}
