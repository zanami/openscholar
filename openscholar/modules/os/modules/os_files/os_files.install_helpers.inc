<?php


function _os_files_install_replace_file_in_text(&$text, $search, $replace) {
  // things to replace:
  // data-fid="n"
  // "fid":"n"
  // src="url"
  // href="url"

  $text = preg_replace('data-fid="'.$search->fid.'"', 'data-fid="'.$replace->fid.'"', $text);
  $text = preg_replace('"fid:"'.$search->fid.'"', '"fid:"'.$replace->fid.'"', $text);
  $text = preg_replace('href="'.file_create_url($search->uri).'"', 'href="'.file_create_url($replace->uri).'"', $text);
  $text = preg_replace('src="'.file_create_url($search->uri).'"', 'src="'.file_create_url($replace->uri).'"', $text);
  // image style images
  $text = preg_replace('src="([^"]*)'.file_target_uri($search->uri).'"', 'src="$1'.file_target_uri($replace->uri).'"', $text);
}