<?php

/**
 * @file
 * Theme related functions for oEmbed Core
 */

/**
 * Theme for all oEmbed output.
 */
function theme_oembed(&$vars) {
  $embed = $vars['embed'];
  $title = $vars['title'];
  $content = $vars['content'];
  return render($content);
}
