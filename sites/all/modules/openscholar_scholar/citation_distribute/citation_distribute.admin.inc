<?php

/**
 * Implementation of cite_distribute_admin()
 * 
 * @return array
 *   returns system setting form
 */
function citation_distribute_admin() {
	$form = array();
  $form['citation_distribute'] = array(
	  '#type' => 'fieldset',
	  '#title' => t('Configure Citation Distribute'),
	  '#collapsible' => FALSE,
	  '#collapsed' => FALSE,
	);

	//module mode 
  $form['citation_distribute']['citation_distribute_module_mode'] = array(
	  '#type' => 'select',
	  '#title' => t('Select the mode for this module'),
	  '#options' => array('api'=> 'API Mode', 'batch' => 'Batch Process Mode', 'per_submission' => 'Per Submission Mode'),
	  '#required' => TRUE,
	  '#default_value' => variable_get('citation_distribute_module_mode','per_submission'),
	  '#description' => t('<strong>API mode</strong> does nothing by default, but allows developers to call Citation Distribute manually.
	      <br><strong>Batch mode</strong> is intended to be run by cron will update all meta files at once in a batch process. 
	      <br><strong>Per Submission mode</strong> (<em>default</em>) will update or create a meta file whenever content submitted or updated.')
   );
   
   //cron limit
   $form['citation_distribute']['citation_distribute_cron_limit'] = array(
     '#type' => 'textfield',
   	 '#title' => 'Batch Size Limit',
   	 '#description' => t('(Batch mode only) Limit how many publications can be submitted per cron run.'),
   	 '#required' => FALSE,
   	 '#default_value' => variable_get('citation_distribute_cron_limit', _CRON_LIMIT),
   );
   
   //list all our plugins, include autoflag checkboxes

   $form['citation_distribute_autoflags'] = array(
     '#title' => t('Automatic Node Flagging'),
     '#type' => 'checkboxes',
     '#description' => t('New publications will automatically be flagged for distribution to:'),
     '#default_value' => array_flip(array_filter(variable_get('citation_distribute_autoflags', array()))),
     '#options' => array(),
   );
   foreach (_citation_distribute_plugins() as $plugin) {
     if (isset($plugin['title'])) {
       $form['citation_distribute_autoflags']['#options'] += array($plugin['handler']['class'] => $plugin['title']);
     }
   }
   
   return system_settings_form($form);
}

/**
 * implementation of form_validate
 * 
 * Ensures that cron limit is a positive integer
 */
function citation_distribute_admin_validate($form, &$form_state) {
	if ((int)$form_state['values']['citation_distribute_cron_limit'] > 0) {
		return;		
	} else {
		form_set_error('', t('Batch size limit must be a positive integer.'));
	}
}

/* 
 * Fetches and returns admin form from a plugin.
 */
function citation_distribute_plugin_admin($args, $plugin) {
  $plugins = _citation_distribute_plugins();
 
	if ($class = ctools_plugin_get_class($plugins[$plugin], 'handler', TRUE)) {
		if ($service = new $class()) {							
		  if (method_exists($service, 'admin_form')) {
				$form = $service->admin_form();
        return system_settings_form($form);
		  }
		}
	}
  
}
 
