<?php

/**
 * @file media_oembed/MediaInternetOEmbedHandler.inc
 *
 * Contains MediaInternetOEmbedHandler.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternetOEmbedHandler extends MediaInternetBaseHandler {

  public $fileObject;

  /**
   * Claim this URL.
   */
  public function claim($embedCode) {
    $matches = array();
    if (oembedcore_get_provider($embedCode, $matches)) {
      return TRUE;
    }
  }

  /**
   * File should not validate if we cannot fetch valid oEmbed data.
   */
  public function validate() {
    $embed = $this->getOEmbed();
    if (!$embed) {
      throw new MediaInternetValidationException(t('Unable to fetch oEmbed data for @url or it is not a valid URL.', array('@url' => $this->embedCode)));
    }
    if (empty($embed['version']) || empty($embed['type']) || intval($embed['version']) != 1) {
      throw new MediaInternetValidationException(t('oEmbed data for @url is invalid.', array('@url' => $this->embedCode)));
    }

    // Validate that response has required properties for its type.

    if (in_array($embed['type'], array('video', 'rich'))) {
      if (!isset($embed['html']) || empty($embed['html'])) {
        throw new MediaInternetValidationException(t('oEmbed response for @url is missing required properties for @type.', array('@url' => $this->embedCode, '@type' => $embed['type'])));
      }
    }

    if ($embed['type'] == 'photo') {
      if (!isset($embed['url']) || empty($embed['url'])) {
        throw new MediaInternetValidationException(t('oEmbed response for @url is missing required properties for @type.', array('@url' => $this->embedCode, '@type' => $embed['type'])));
      }
    }
  }

  /**
   * Returns a file object which can be used for validation
   *
   * @return StdClass
   */
  public function getFileObject() {
    if (!$this->fileObject) {
      $uri = 'oembed://'. drupal_encode_path($this->embedCode);
      $file = file_uri_to_object($uri);
      if (empty($file->type)) {
        $file->type = file_get_type($file);
      }
      $this->fileObject = $file;
    }
    return $this->fileObject;
  }

  /**
   * Before the file has been saved, implementors may do additional operations.
   */
  public function preSave(&$file_obj) {
    $url = rawurldecode(substr($file_obj->uri, 9));
    $embed = $this->getOEmbed();
    $file_obj->filename = truncate_utf8(empty($embed['title']) ? oembedcore_alt_attr($embed) : $embed['title'], 255);

    // If a user enters a duplicate embed code, the object will be saved again.
    // Set the timestamp to the current time, so that the media item shows up
    // at the top of the media library, where they would expect to see it.
    $file_obj->timestamp = REQUEST_TIME;
  }

  public function postSave(&$file_obj) {
    $embed = $this->getOEmbed();

    if ($file_obj->type == 'image') {
      $file_obj->image_dimensions = array(
        'width' => $embed['width'],
        'height' => $embed['height'],
      );
      db_merge('image_dimensions')
        ->key(array('fid' => $file_obj->fid))
        ->fields(array(
          'width' => $embed['width'],
          'height' => $embed['height'],
        ))
        ->execute();
    }
  }

  /**
   * Returns an array representation of the oEmbed data, or NULL on failure.
   *
   * An example project that calls this method is Media Browser Plus
   * (http://drupal.org/project/media_browser_plus). Other projects may do so
   * in the future.
   */
  public function getOEmbed() {
    $embed = oembedcore_oembed_data($this->embedCode);
    return $embed ? (array) $embed : NULL;
  }
}
