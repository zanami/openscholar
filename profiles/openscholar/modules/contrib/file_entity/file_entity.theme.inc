<?php

/**
 * @file
 * Theme callbacks for the file entity module.
 */

/**
 * A copy of theme_file_file_link() that makes the link point to file/[fid].
 *
 * @see theme_file_file_link()
 * @return type
 */
function theme_file_entity_file_link($variables) {
  $file = $variables['file'];
  $icon_directory = $variables['icon_directory'];

  $url = 'file/' . $file->fid;
  $icon = theme('file_icon', array('file' => $file, 'icon_directory' => $icon_directory));

  // Set options as per anchor format described at
  // http://microformats.org/wiki/file-format-examples
  $options = array(
    'attributes' => array(
      'type' => $file->filemime . '; length=' . $file->filesize,
    ),
  );

  // Use the description as the link text if available.
  if (empty($file->description)) {
    $link_text = $file->filename;
  }
  else {
    $link_text = $file->description;
    $options['attributes']['title'] = check_plain($file->filename);
  }

  if (isset($file->override['wysiwyg'])) {
    $icon = file_icon_path($file, $icon_directory);
    $icon_size = getimagesize($icon);
    $icon_image = imagecreatefrompng($icon);
    $icon_size += array(
      'width' => $icon_size[0],
      'height' => $icon_size[1],
    );

    $image = imagecreate(200, $icon_size['height'] + 4);
    imagecopy($image, $icon_image, 2, 2, 0, 0, $icon_size['width'], $icon_size['height']);

    $str = 'Placeholder Text';
    $font = 3;
    $offset = $icon_size['width'] + 2;
    $width = imagefontwidth($font);
    $color = imagecolorallocate($image, 0, 0, 0);
    for ($i=0, $l=strlen($str); $i<$l; $i++) {
      $c = $str[$i];
      imagechar($image, $font, $offset + $width*$i, 2, $c, $color);
    }

    // capture the image data
    ob_start();
    imagepng($image);
    $data = ob_get_contents();
    ob_end_clean();

    return '<img src="data:image/png;base64,'.base64_encode($data).'">';
  }
  return '<span class="file">' . $icon . ' ' . l($link_text, $url, $options) . '</span>';
}