<?php

/**
 * @file
 * Form validate and submit callbacks that only happen for User 1.
 */

/**
 * Form validate callback; adds purl validation if domain value is set.
 */
function vsite_domain_cp_settings_domain_validate($form, $form_state) {
  // First, exits if purl_form_validate() fails.
  if (strlen($form['value']['#value'])) {
    if (!purl_form_validate($form)) {
      // purl_form_validate() already handles the form_set_error().
      return FALSE;
    }
  }

  // Prepares form values to validate more...
  $custom_domain = $form_state['values']['vsite_domain_name']['value'];
  $shared = $form_state['values']['vsite_domain_name']['vsite_domain_shared'];

  // Validates owned domains:
  if ($shared == 1) {
    // Checks that there is a custom domain name value filled.
    if (strlen($custom_domain) == 0) {
      form_set_error('vsite_domain_shared', t('You must enter a custom domain in order to use <em>Owned domain name</em> option.'));
    }
  }

  // Validates shared and owned domains.
  // Checks that the custom domain is not the same as purl_base_domain.
  // Note: $base_url and purl_base_domain may differ, but $base_url is a better
  // default value than ''.
  $purl_base_domain = variable_get('purl_base_domain', '');
  $purl_base_domain = preg_replace("(https?://)", "", $purl_base_domain);
  if (strtolower($custom_domain) == strtolower($purl_base_domain)) {
    form_set_error('vsite_domain_name', t('You may not use the sitewide URL as a custom url.'));
  }

  return TRUE;
}

/**
 * Form submit callback; updates purl table when vsite_domain_name changes.
 */
function vsite_domain_cp_settings_domain_submit(&$form, &$form_state) {
  // Note: this submit callback is only added to the CP > Settings form for
  // users with the "change vsite domain" permission -- currently only User 1.
  // Additionally, costly functions first check that values have been modified
  // in order to avoid needless expensive cache flushing & database operations.

  // Returns an array containing the original and new values, and bool modified.
  $domain = _vsite_domain_check_domain($form, $form_state);
  // Consider sites without custom domains to be "shared" (use purl modifier).
  if (!strlen($domain['new'])) {
    $form_state['values']['vsite_domain_name']['vsite_domain_shared'] = 1;
  }
  // Returns an array containing the original and new values, and bool modified.
  $shared = _vsite_domain_check_shared($form, $form_state);

  if ($shared['modified']) {
    dpm('processing modified!');
  }
  if ($domain['modified']) {
    dpm('domain modified!');
  }

  // Stashes the form_state['values'] array before we modify it.
  $values = $form_state['values'];

  // Saves "vsite_domain_name" and "vsite_domain_shared" in variable table.
  // We place the values into the $form_state['values'] array like this
  // in order so that the settings form automatically updates the variables.
  $form_state['values']['vsite_domain_shared'] = $form_state['values']['vsite_domain_name']['vsite_domain_shared'];
  $form_state['values']['vsite_domain_name'] = $form_state['values']['vsite_domain_name']['value'];

  // Exits if the domain name variables did not changed...
  if (!$shared['modified'] && !$domain['modified']) {
    return;
  }

  // The URL has changed. Updates the vsite node's Group path field.
  $group_path = _vsite_domain_set_group_path($values['vsite_domain_name']);
  dpm($group_path, "new path");

  // If this site is going from owned to the sitewide URL (standard setup)...
  if (($domain['modified'] && !$domain['new'])
  // Or from the standard to a shared custom domain...
  || (!$shared['modified'] && !$domain['original'] && $domain['new'])) {

    // Redirects to use the original sitewide domain in destination URL.
    $destination = isset($form_state['redirect']) ? $form_state['redirect'] : request_path();
    // If we started at a shared domain, first removes the (now extra) purl from
    // the destination URL.
    if (!$shared['modified'] || (!$shared['modified'] && $domain['new'])) {
      $destination = implode("/", array_slice(explode("/", $destination), 1));
      dpm('destination -= purl');
    }
    // Prepend with "[site:url]/$purl".
    $form_state['redirect'] = "$group_path/$destination";
    dpm($form_state['redirect'], 'redirect path+destination');
  }

  // Case J
  if ($shared['original'] && $domain['original'] && $shared['new'] && $domain['new']) {

  }

  // Returns the type of purl change required, or FALSE if no change required.
  $purl_change = _vsite_domain_purl_change($domain, $shared);
  if (!$purl_change) {
    return;
  }
  dpm($purl_change);

  // Prepares a purl modifier array to be saved.
  // Case H:
  if ($shared['modified'] && $shared['new'] && $domain['new'] && $domain['modified']) {
    $modifier = array(
      'provider' => $values['vsite_domain_name']['provider'],
      'id' => $values['vsite_domain_name']['id'],
      'value' => $domain['original'],
    );
  }
  else {
    $modifier = array(
      'provider' => $values['vsite_domain_name']['provider'],
      'id' => $values['vsite_domain_name']['id'],
      'value' => $values['vsite_domain_name']['value'],
    );
  }

  // Vsite domain name settings can be in 3 basic states:
  // - 'standard': Uses the sitewide base URL and a purl modifier. Default.
  // - 'owned': Uses a custom domain name and no purl modifier. Less common.
  // - 'shared': Uses a custom domain name with a purl modifier. Rare.
  //
  // The following makes certain changes whenever a site goes to or leaves
  // the "owned" domain name state-- invoking purl_save() and purl_delete().
  switch ($purl_change) {
    // If this vsite is updated or going to an owned vsite from anything else...
    case 'update':
    case 'create':
      // Creates or updates the purl record.
      $status = purl_save($modifier);
      break;

    // If this vsite is going away from an owned vsite to anything else...
    case 'delete':
      // Deletes the existing purl record.
      $status = purl_delete($modifier);
      break;
  }

  // Cache "purl_modifiers_en" needs to be cleared so that the new purl
  // processor is recognized.
  // Case: D
  if (!$domain['modified'] && $shared['new']) {
    cache_clear_all('purl_modifiers_en', 'cache', TRUE);
    dpm('cache flushed');
  }
  // Case: G
  if (!$shared['original'] && !$shared['new']) {
    cache_clear_all('purl_modifiers_en', 'cache', TRUE);
    dpm('test case G cache flush');
    $destination = isset($form_state['redirect']) ? $form_state['redirect'] : request_path();
    $form_state['redirect'] = "$group_path/$destination";
    dpm('test case G redirect path+dest');
  }

  // If this is going from owned to a shared custom domain...
  // Case: D
  if (($shared['modified'] && $shared['new'] && $domain['new'] && !$domain['modified'])) {
  // Or from one shared custom domain to another
  //|| (!$shared['modified'] && $shared['new'] && $domain['new'])) {
    // Flush more caches!
    //cache_clear_all('*', 'cache');
    // Redirects to include the new purl modifier in the URL.
    $destination = isset($form_state['redirect']) ? $form_state['redirect'] : request_path();
    $purl = vsite_get_purl();
    $form_state['redirect'] = "$purl/$destination";
    dpm($form_state['redirect'], 'redirect purl+destination');
  }
  // Case H:
  if ($shared['modified'] && $shared['new'] && $domain['new'] && $domain['modified']) {
    cache_clear_all('purl_modifiers_en', 'cache', TRUE);
    dpm('test case H cache flush');
    $destination = isset($form_state['redirect']) ? $form_state['redirect'] : request_path();
    $purl = vsite_get_purl();
    $form_state['redirect'] = "$group_path/$destination";
    dpm($form_state['redirect'], 'Case H redirect path+purl+dest');
  }
}

/**
 * Returns the type of purl change required, or FALSE if no change required.
 *
 * @param array $domain
 *   An indexed array representing the vsite_domain_name var, containing:
 *   - 'modified': (bool) TRUE only if the original and new values differ.
 *   - 'original': (string) the original value from the form, i.e. "domain.com"
 *   - 'new': (string) the new value from the form, i.e. "domain.com"
 * @param array $shared
 *   An indexed array representing the vsite_domain_shared var, containing:
 *   - 'modified': (bool) TRUE only if the original and new values differ.
 *   - 'original': (string) the original value from the form, accepts '1' or '0'
 *   - 'new': (string) the new value from the form, accepts '1' or '0'
 *
 * @return mixed $change_type
 *   Returns FALSE if the state of this form submit requires no purl changes.
 *   Otherwise, returns the type of change, one of: 'update', 'create', 'delete'
 */
function _vsite_domain_purl_change($domain, $shared) {
  // Was this vsite at an owned domain before this form submit?
  $owned_before = (bool) ($shared['original'] == 0 && strlen($domain['original']));
  // Is this vsite going to be at an owned domain after this form submit?
  $owned_after = (bool) ($shared['new'] == 0 && strlen($domain['new']));

  // Is this vsite changing from an owned domain to a shared domain?
  // Case: I
  if (!$owned_before && $owned_after) {
    return 'create';
  }
  // Is this vsite updating it's domain name and remaining an owned domain?
  else if ($owned_before && $owned_after && $domain['modified']) {
    return 'update';
  }
  // Is this vsite changing from a shared domain to an owned domain?
  else if ($owned_before && !$owned_after) {
    return 'delete';
  }

  // Only makes purl database changes in the above 3 cases.
  return FALSE;
}

/**
 * Returns the type of purl change required, or FALSE if no change required.
 *
 * @param array $domain
 *   An indexed array representing the vsite_domain_name var, containing:
 *   - 'modified': (bool) TRUE only if the original and new values differ.
 *   - 'original': (string) the original value from the form, i.e. "domain.com"
 *   - 'new': (string) the new value from the form, i.e. "domain.com"
 * @param array $shared
 *   An indexed array representing the vsite_domain_shared var, containing:
 *   - 'modified': (bool) TRUE only if the original and new values differ.
 *   - 'original': (string) the original value from the form, accepts '1' or '0'
 *   - 'new': (string) the new value from the form, accepts '1' or '0'
 *
 * @return mixed $change_type
 *   Returns FALSE if the state of this form submit requires no purl changes.
 *   Otherwise, returns the type of change, one of: 'update', 'create', 'delete'
 */
function _vsite_domain_submit_actions($domain, $shared) {
  $actions = array();

  // What purl updates need to be run?
  // Purl needs to be updated when someone goes from or to an "owned" domain--
  // that is, a custom domain name with "share domain name" turned off.
  // If the site is going to AND from an owned domain, we only need to make a
  // change if the custom domain name has changed.

  // Was this vsite at an owned domain before this form submit?
  $owned_before = (bool) ($shared['original'] == 0 && strlen($domain['original']));
  // Is this vsite going to be at an owned domain after this form submit?
  $owned_after = (bool) ($shared['new'] == 0 && strlen($domain['new']));

  // Is this vsite changing from an owned domain to a shared domain?
  // Examples:
  //   "[site:url]/foo"     --> "http://bar.com"
  //   "http://bar.com/foo" --> "http://bar.com"
  //   "http://baz.com/foo" --> "http://bar.com"
  if (!$owned_before && $owned_after) {
    $actions['purl_callback'] = 'purl_save';
  }
  // Is this vsite updating it's domain name and remaining an owned domain?
  // Example:
  //   "http://bar.com" --> "http://baz.com"
  else if ($owned_before && $owned_after && $domain['modified']) {
    $actions['purl_callback'] = 'purl_save';
  }
  // Is this vsite changing from a shared domain to an owned domain?
  // Examples:
  //   "http://bar.com" --> "[site:url]/foo"
  //   "http://bar.com" --> "http://bar.com/foo"
  else if ($owned_before && !$owned_after) {
    $actions['purl_callback'] = 'purl_delete';
    if (!$domain['new']) {
      $actions['cache'] = array('purl_modifiers_en');
    }
  }

  // Only makes purl database changes in the above 3 cases.
  return $actions;
}

/**
 * Submit callback; updates and saves a vsite node Group path field.
 *
 * @param array $values
 *   An indexed array containing:
 *   - 'value': The domain name value without http://
 *   - 'vsite_domain_shared': [sic] Owned/group domain, accepted values:
 *     - '0': Owned
 *     - '1': Group
 * @param mixed $vsite
 *   Accepts SID as integer/string, or loaded vsite object. Defaults to current.
 *
 * @see vsite_domain_cp_settings_domain_submit()
 */
function _vsite_domain_set_group_path($values, $vsite = NULL) {
  // Handles optional param
  if (is_numeric($vsite)) {
    $vsite = vsite_get_vsite($vsite);
  }
  else if (!$vsite) {
    $vsite = vsite_get_vsite();
  }

  // No need to check here whether these are new values--
  // This function is only called if one of these two variables has changed.
  $custom_domain = $values['value'];
  $processing = $values['vsite_domain_shared'];

  // Checks whether to use global $base_url or custom for Group path.
  if (strlen($custom_domain)) {
    // Prepends scheme to front of domain name string.
    // Note: Currently does not handle HTTPS scheme.
    $group_path = "http://$custom_domain";
  }
  else {
    // Stores the global purl base domain as a token.
    $group_path = '[site:url]';
    // Prepares separate return value without token.
    $return = variable_get('purl_base_domain', '');
  }

  // Optionally appends the purl modifier to the end of the Group path string.
  if ($processing == 1) {
    // Only loads the purl modifier if we will add it to the URL string.
    $purl = vsite_get_purl($vsite);
    $group_path = "$group_path/$purl";
    // Updates return value if different from stored value.
    if (isset($return)) {
      $return = "$return/$purl";
    }
  }

  // Saves the Group path field value on the vsite node.
  $wrapper = entity_metadata_wrapper('node', $vsite->group);
  $field_value = $wrapper->field_group_path->value();
  $field_value['url'] = $group_path;
  $wrapper->field_group_path->set($field_value);
  $wrapper->save();

  // Returns the new URL without tokens.
  return isset($return) ? $return : $field_value['url'];
}

/**
 * Submit callback; Checks whether vsite_domain_shared value is changed.
 *
 * @return array
 *
 * @see vsite_domain_cp_settings_domain_submit()
 */
function _vsite_domain_check_shared($form, $form_state) {
  $original_path = array('vsite_domain_name', 'vsite_domain_shared', '#default_value');
  $new_path = array('values', 'vsite_domain_name', 'vsite_domain_shared');
  return _vsite_domain_form_value_modified($form['domain'], $form_state, $original_path, $new_path);
}

/**
 * Submit callback; Checks whether the vsite_domain_name value is changed.
 *
 * @return array
 *
 * @see vsite_domain_cp_settings_domain_submit()
 */
function _vsite_domain_check_domain(&$form, &$form_state) {
  $original_path = array('domain', 'vsite_domain_name', 'value', '#default_value');
  $new_path = array('values', 'vsite_domain_name', 'value');
  return _vsite_domain_form_value_modified($form, $form_state, $original_path, $new_path);
}

/**
 * Returns TRUE if the specified element has two set and different values.
 *
 * Returns FALSE if either element path is undefined, or if the two values are
 * equal using the "==" operator.
 *
 * @param array $original_array
 *   A multi-leveled indexed Drupal $form array
 * @param array $new_array
 *   A multi-leveled indexed Drupal $form_state array
 * @param array $original_path
 *   An ordered array of keys to access the original value in $original_array.
 * @param array $new_path
 *   An ordered array of keys to access the new value in $new_array.
 *
 * @return bool
 *   Returns TRUE only if both paths are defined until at least the penultimate
 *   key value, and both accessed values are equal using the `==` operator.
 *
 * @see _vsite_domain_check_form_value()
 */
function _vsite_domain_form_value_modified($original_array, $new_array, $original_path, $new_path) {
  // Attempts to access both values in their respective arrays...
  $original_val = _vsite_domain_check_form_value($original_array, $original_path);
  $new_val      = _vsite_domain_check_form_value($new_array, $new_path);

  // Returns FALSE if either of the values were inaccessible.
  if ($original_val === FALSE || $new_val === FALSE) {
    return FALSE;
  }

  // Returns TRUE if the values are different, otherwise they are unchanged.
  return array(
    'modified' => (bool) ($new_val != $original_val),
    'original' => $original_val,
    'new'      => $new_val,
  );
}

/**
 * Returns the value of an index at a given path, or FALSE if undefined.
 *
 * Note: Returns '' if only the last value of the path is undefined.
 *
 * @param array $array
 *   A multi-leveled indexed array, i.e. a Drupal $form or $form_state array
 * @param array $path
 *   An ordered array of string/int keys to access a value in the array.
 *
 * @return mixed
 *   Any possible value stored in a the array, or FALSE if any but the last
 *   key in the $path array was undefined.
 *
 * @see _vsite_domain_form_value_modified()
 */
function _vsite_domain_check_form_value($array, $path) {
  // Attempts to traverse the indexed array using the specified path...
  while (!empty($path)) {
    $next = array_shift($path);
    if (!isset($array[$next])) {
      // If this element is the last element, treats undefined key as value '';
      if (empty($path)) {
        return '';
      }
      // Returns FALSE if the element does not exist on the $form array.
      return FALSE;
    }
    //
    $array = $array[$next];
  }

  // Returns the successfully accessed value in the array.
  return $array;
}