<?php

/**
 * Implements hook_file_default_displays().
 *
 * Provides default display configurations for media types.
 *
 * @see file_entity_schema()
 */
function media_oembed_file_default_displays() {
  $default_displays = array();

  $default_styles = array();
  foreach(file_type_get_enabled_types() as $name => $type) {
    // For default and Large view modes, use the default width and height from
    // hook_file_formatter_info().
    $default_styles[$name . '__default__oembed'] = array();
    $default_styles[$name . '__default__oembed_thumbnail'] = array();

    // For the Preview view mode, match the width and height of the square
    // thumbnail in media_image_default_styles().
    $default_styles[$name . '__preview__oembed_thumbnail'] = array('width' => '180', 'height' => '180');

    // For the Original view mode, set width and height to empty, so that they
    // use what's returned by the oEmbed response.
    $default_styles[$name . '__full__oembed'] = array('width' => '', 'height' => '');
  }

  foreach ($default_styles as $display_name => $settings) {
    $default_displays[$display_name] = (object) array(
      'api_version' => 1,
      'name' => $display_name,
      'status' => 1,
      'weight' => 0,
      'settings' => $settings,
    );
  }

  return $default_displays;
}
