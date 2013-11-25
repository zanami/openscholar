<?php
/**
 * Defines a form to display the various settings that have nowhere else to go
 */
function cp_settings_form($form_state) {
  $form = array();
  $form['#submit'] = array();
  $description = t('Customize general settings for your site, like privacy, accessibility and connected web services.');
  $form['cp_settings_description'] = array(
    '#type' => 'markup',
    '#markup' => "<p>{$description}</p>",
    '#weight' => -11,
  );
  $form['cp_settings_tabs'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => -10,
  );

  $settings = cp_get_setting_forms();
  foreach ($settings as $key => $s) {
    if (!isset($s['form'])) continue;

    // if the setting defines a group for itself,
    // that group should be created and the form placed
    // in the group
    if (isset($s['group'])) {
      $id = $s['group']['#id'];
      if (!isset($form[$id])) {
        $form[$id] = $s['group'];
        $form[$id]['#type'] = 'fieldset';
      }

      $form[$id][$key] = $s['form'];
      $form[$id]['#group'] = 'cp_settings_tabs';
    }
    else {
      $form[$key] = $s['form'];
      $form['#group'] = 'cp_settings_tabs';
    }

    // if the form includes its own submit handlers,
    // merge them into the existing
    if (isset($s['submit'])) {
      $form['#submit'] = array_merge($form['#submit'], $s['submit']);
    }
  }

  return system_settings_form($form);
}
