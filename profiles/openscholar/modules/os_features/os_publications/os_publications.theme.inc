<?php
/**
 * The "Teaser" View
 * Should just display the citation and book cover if there is one
 * @param unknown_type $vars
 */
function theme_os_publications_entry(&$vars) {

}

/**
 * Full Node View
 * Displays the citation in a field that can be clicked to select all of it
 * Also displays abstract, full text and notes if enabled.
 * @param unknown_type $vars
 */
function theme_os_publications_tabular(&$vars) {
  $node = $vars['node'];
  $base = $vars['base'];

  $fields = _biblio_get_field_information($node->biblio_type, TRUE);
  _biblio_localize_fields($fields);

  /* This field hasn't been added yet
  if ($node->biblio_url) {
    $attrib = (variable_get('biblio_links_target_new_window', null)) ? array('target' => '_blank') : array();
    $url_title = db_result(db_query('SELECT biblio_url_title FROM {biblio} WHERE nid = %d', $node->nid));
    $url_title = (isset($url_title) && $url_title) ? $url_title : 'Website';
    $node->biblio_url = l($url_title, $node->biblio_url, $attrib);
  }
  */

  $citation = _os_publications_plain_citation($node);

  $hidden = array(
    'biblio_pages',
    'biblio_publisher',
    'biblio_volume',
    'biblio_issue',
    'biblio_year',
    'biblio_place_published',
    'biblio_type_name',
    'biblio_secondary_title'
  );

  foreach ($fields as $f) {

  }

  return $citation;
}

/**
 * Get the citation for a biblio node as just plain text
 */
function _os_publications_plain_citation($node) {
  $style = biblio_get_style();
  $base  = variable_get('biblio_base', 'biblio');

  // Note that this is a working paper after the title and before the year
  if ($style == 'chicago' && in_array($node->biblio_type, array(1000)) && empty($node->biblio_publisher)){
    $node->biblio_publisher = "Working Paper";
  }

  $citation = theme('biblio_style', array('node' => $node, 'base' => $base, 'style' => $style));

  // add url to citation
  if (function_exists('shorten_url') && variable_get('os_publications_shorten_citations', 0)) {
    global $base_url;
    /* replace punctuation at end of citation with period.
     * biblio_coins follow end of citation and block an easy RE match, so we're appending them.
     */
    $coins = filter_xss($node->biblio_coins, array('span'));
    $re = '/' .  '[!-~] (<\/\w*>)?' . preg_quote($coins, '/') . '/';

    if (preg_match( $re, $citation)) {
      $citation = preg_replace($re, ", </span>$coins", $citation);
    }

    $citation .= ' copy at ' . shorten_url(url('node/'.$node->nid, array('absolute'=>TRUE)));
  }

  $citation = strip_tags($citation);
  drupal_add_js('function selectText(ele) {
	try {
  	var div = document.createRange();
    div.setStartBefore(ele);
    div.setEndAfter(ele) ;
  	window.getSelection().addRange(div);
  }
  catch (e) {
  	// we\'re in internet explorer
  	div = document.selection.createRange();
  	div.moveToElementText(ele);
  	div.select();
  }
}','inline');
  $citation = "<dl><dt>Citation:</dt><dd class=\"biblio-citation\" onclick=\"if (event.target) selectText(event.target); else selectText(this);\">{$citation}</dd></dl>";

  return $citation;
}