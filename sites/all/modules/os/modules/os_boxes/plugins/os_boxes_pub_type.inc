<?php

/**
 * @file
 * The class for "Embed" type widgets.
 */

class os_boxes_pub_type extends os_boxes_default {
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();

    $options['whitelist'] = array('all');
    $options['display_num'] = 1;

    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){
    $q = db_query("SELECT tid, name FROM {biblio_types} WHERE tid > 0 AND visible = 1");
    $options = array('all' => 'Select All');
    $value = $this->options['whitelist'];
    $all = in_array('all', $value);
    while ($r = db_fetch_array($q)) {
      $options[$r['tid']] = $r['name'];
      if ($all) $value[] = $r['tid'];
    }

    $form['whitelist'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => $value,
      '#title' => t('Show the following types:'),
      '#attributes' => array('class' => 'pub_type'),
      '#description' => t('Select the types you want to appear in the widget.')
    );

    $form['display_num'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display number of items in category'),
      '#default_value' => $this->options['display_num'],
    );
    drupal_add_js(drupal_get_path('module', 'os_boxes').'/theme/js/selectAll.js');

    $form += parent::options_form();
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    //@todo
    $block = parent::render();

  	$args = array();
  	$join = "";
  	$where = array();
  	$vsite = vsite_get_vsite();
  	if ($vsite){
  		$args[] = $vsite -> group-> nid;
  		$join .= "INNER JOIN {og_ancestry} oga ON n.nid = oga.nid";
  		$where[] = "oga.group_nid = %n";
  	}

  	if (!in_array('all', $this->options['whitelist']) && count($this->options['whitelist'])) {
  	  foreach ($this->options['whitelist'] as &$wl) {
  	    $wl = "'".$wl."'";
  	  }
  	  $args[] = implode(", ", $this->options['whitelist']);
  	  $where[] = "tid IN (%s)";
  	}

    $s = "SELECT COUNT(DISTINCT(n.nid)) AS num, tid AS path, name AS label FROM {biblio} n INNER JOIN {biblio_types} bt ON (n.biblio_type = bt.tid) ".$join.' WHERE '.implode(' AND ', $where).' GROUP BY label ORDER BY label ASC';
    $q = db_query($s, $args);

    $i = 0;
    $plurals = array(
    	'Software',
    	'Data',
    	'Conference Proceedings',
    	'Thesis',
    	'Unpublished',
    	'Miscellaneous',
    	'Audiovisual',
    	'Artwork'
    );
    $content = array();
    while ($row = db_fetch_array($q)) {
      if ($this->options['type'] == 'type' && !in_array($row['label'], $plurals)) $row['label'] .= "s"; //FIX THE PLURAL

      $content[] = '<li>'.l($row['label'],"publication/".$this->options['type']."/".$row['path']).($this->options['display_num']?' ('.$row['num'].')</li>':'');
    }

    if (count($content)) {
      $block['content'] = '<ul>'.implode('', $content).'</ul>';
    }

    return $block;
  }

  /**
   * Return the cache parameters for this box
   */
  public function cache_info(){

    //Clear when bio nodes are saved
    $cache_key = array(
      'os_og' => array('current' => 'current'),
      'node' => array('biblio' => 'biblio'),
    );

    return array('type' => BLOCK_CACHE_CONTENT_CACHE, 'keys' => $cache_key);
  }

}