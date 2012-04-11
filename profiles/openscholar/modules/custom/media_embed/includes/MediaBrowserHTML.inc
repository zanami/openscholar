<?php

/**
 * @file
 * Media browser embed plugin.
 */

class MediaBrowserHTML extends MediaBrowserPlugin {

  function view() {
    // @todo: implement the multiselect argument here.
    $from_web_form = drupal_get_form('media_embed_add',  $this->params['types'], $this->params['multiselect']);
    return array(
      'form' => $from_web_form,
    );
  }

}
