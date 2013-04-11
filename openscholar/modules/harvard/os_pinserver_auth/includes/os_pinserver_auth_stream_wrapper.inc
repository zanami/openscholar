<?php

/**
 * Extension of Drupal private (private://) stream wrapper class.
 *
 * Provides support for loading private files via pinserver.
 */
class OSPinserverPrivateStreamWrapper extends DrupalPrivateStreamWrapper {

  /**
   * Overrides getExternalUrl().
   *
   * Return the HTML URI of a private file.
   */
  function getExternalUrl() {
    //Should we serve files via the pinserver URL?
    if(!variable_get('os_files_private_pinserver', 0)){
      return parent::getExternalUrl();
    }

    $path = str_replace('\\', '/', $this->getTarget());
    return url(variable_get('os_pinserver_auth_fast_path','os_fast').'/files/' . $path, array('absolute' => TRUE));
  }
}
