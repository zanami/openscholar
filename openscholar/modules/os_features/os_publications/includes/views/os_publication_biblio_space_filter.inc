<?php

/**
 * Spaces view filter handler.
 */
class os_publication_biblio_space_filter extends spaces_handler_filter_spaces_current {

  /**
   * Meta query handler which delegates query building to the implementing spaces.
   */
  function query() {
    if (!$space = spaces_get_space()) {
      return;
    }

    $alias = $this->query->add_table('og_membership', $this->relationship);
    $this->query->add_where($this->relationship, "$alias.gid", $space->id);
  }
}
