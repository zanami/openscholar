<?php

/**
 * @file
 * The class for "Publication By Year" type widgets.
 */

class os_boxes_pub_ca extends os_boxes_default {
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();

    $options['display_num'] = 1;

    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){

    $form['display_num'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display number of items in category:'),
      '#default_value' => $this->options['display_num'],
    );

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

    $s = "SELECT COUNT(DISTINCT(n.nid)) AS num, bcd.lastname AS path, bcd.lastname AS label FROM {node} n INNER JOIN {biblio_contributor} bc ON (n.vid = bc.vid) INNER JOIN {biblio_contributor_data} bcd ON (bc.cid = bcd.cid) ".$join.' WHERE '.implode(' AND ', $where).' GROUP BY label ORDER by label ASC';
    $q = db_query($s, $args);

    $content = array();
    while ($row = db_fetch_array($q)) {
      $content[] = '<li>'.l($row['label'],"publications/contributors/".$row['path']).($this->options['display_num']?' ('.$row['num'].')</li>':'');
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