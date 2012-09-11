<?php
// $Id$


/**
 * @file
 * Common functions for working with node content in OpenScholar.
 */

class OpenscholarNodeFieldQuery extends EntityFieldQuery {
  // define some defaults for the class
  public function __construct() {
    // now we don't need to define these over and over anymore
    $this
      ->entityCondition('entity_type', 'node')
      ->propertyCondition('status', 1);

    if (!module_exists('vsite')) {
      return;
    }

    $space = spaces_get_space();
    if (!is_numeric($space->id)) {
      return;
    }

    $this
      ->fieldCondition(OG_AUDIENCE_FIELD, 'target_id', $space->id);
  }
}
