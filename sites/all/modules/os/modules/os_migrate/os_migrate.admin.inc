<?php 

/**
 * @file os_migrate.admin.inc
 * 
 * Provides form for OpenScholar migrate settings
 */

/**
 * os_migrate's admin form
 */
function os_migrate_admin() {
  $form = array();
  $form['os_migrate_database'] = array(
    '#title' => t('Migration database'),
    '#description' => t('Drupal 6 database from which to migrate data.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('os_migrate_database', ''),
  );

  $form['#validate'][] = 'os_migrate_admin_validate';
  
  $form['settings_help'] = array(
    '#title' => t('settings.php configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#type' => 'fieldset',
    '#description' => <<<EOF
<p>Your settings.php file needs to define a connection to the Drupal 6 database.  Please ensure that
in addition to the default connection to your Drupal 7 database, settings.php includes a connection
to the database you specifed.  It will look something like the text below, but with the bold values
replaced with those applicable to your DB.  </p>

The Migration database setting on this page corresponds to the <strong>database</strong> entry below 
as specified in your settings.php file.

<pre>
\$databases['<strong>database</strong>']['default'] = array(
  'database' => '<strong>db name</strong>',
  'username' => '<strong>db user</strong>',
  'password' => '<strong>db password</strong>',
  'host' => 'localhost',
  'port' => '', 
  'driver' => 'mysql',
  'prefix' => '', 
);
</pre>
EOF
  );

  
  return system_settings_form($form);
}

/**
 * Validates os_migrate's admin form.
 * Tries to connect to supplied db.  
 */
function os_migrate_admin_validate($form, &$form_state) {
  $os_migrate_database = $form_state['values']['os_migrate_database'];
  
  // try to connect to database to confirm it's set up correctly.
  if (strlen($os_migrate_database)) {
    try {
      Database::getConnection('default', $os_migrate_database);
      Database::closeConnection('default', $os_migrate_database);
      drupal_set_message('Can connect to migration database.');
    } catch (DatabaseConnectionNotDefinedException $e){
      form_set_error('os_migrate_database', t('Unable to connect to specified database.  Check your settings.php configuration.'));
    }
  }  
}