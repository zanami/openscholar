<?php
function cp_preprocess_overlay(&$vars) {
  // we never want these. They look awful
 $vars['tabs'] = false;
}