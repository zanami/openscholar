<?php

/**
 * @file
 * Provides class that expires files.
 */

class OSExpireFile extends ExpireFile {

  /**
   * Executes expiration actions for file.
   *
   * @param $file
   *   File object.
   *
   * @param $action
   *   Action that has been executed.
   *
   * @param $skip_action_check
   *   Shows whether should we check executed action or just expire node.
   */
   function expire($file, $action, $skip_action_check = FALSE) {

    //Have a feeling we are going to need to do somthing special here...
    parent::expire($node, $action, $skip_action_check);
  }
}
