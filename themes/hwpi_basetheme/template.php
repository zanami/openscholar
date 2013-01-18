<?php

// Rebuild the theme data.
//system_rebuild_theme_data();
//drupal_theme_rebuild();

function hwpi_basetheme_preprocess_html(&$vars) {

  $scripts = array(
    'matchMedia.js', // matchMedia polyfill for older browsers
    'eq.js',         // Equalheights script
    'eq-os.js',      // Call Equalheights for specific elements
  );
  foreach ($scripts as $script) {
    // See load.inc in AT Core, load_subtheme_script() is a wrapper for drupal_add_js()
    load_subtheme_script('js/' . $script, 'hwpi_basetheme', 'header', $weight = NULL);
  }

}
