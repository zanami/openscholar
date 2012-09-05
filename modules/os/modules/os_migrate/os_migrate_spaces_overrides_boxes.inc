<?php 

class SpacesOverridesBoxesMigration extends SpacesOverridesTableMigration {
  
  public function __construct() {
    $this->source_object = $this->dest_object = 'boxes';
    $this->dependencies = array('ModalImageNodeOS', 'SpacesOverridesContext'); //images have to be copied before modal boxes can migrate.
    parent::__construct();
  }
  
  public function query() {
    $query = parent::query();
    $query->condition('object_id', '', '!=');
    
    
    //$query->condition('value', '%os_boxes_node%', 'LIKE'); //testing the port function
    //$query->condition('value', '%os_boxes_modal_image%', 'LIKE'); //testing the port function
 //   $query->condition('value', '%manual%', 'LIKE');
    
    return $query;
  }
  
  
  
  public function prepareRow(&$row) {
    if (($value = @unserialize($row->value)) === FALSE) {
      if (strlen($row->value) > 65535) {
        //single image saved some images inline (ie img src='data:image:png....).  these were truncated in db and don't function
        parent::saveMessage(t('Box too big, source data truncated.  Ignoring.  id=@id', array('@id'=>$row->id), MigrationBase::MESSAGE_NOTICE));
      } else {        
        parent::saveMessage(t('Can\'t unserialize box: id=@id, object_id=@object_id', 
          array('@id' => $row->id, '@object_id' => $row->object_id)), MigrationBase::MESSAGE_WARNING);
      }
      return FALSE;
    }
    
    
    if ($box = $this->_update_box($value, $row->id)) {
      $value = $box;
    } else {
      parent::saveMessage(t('Could not migrate box: @key', array('@key'=>$value->plugin_key)), MigrationBase::MESSAGE_NOTICE); 
      return FALSE;
    }
    
    if (!$this->_box_key_exists($box->plugin_key)) {
      parent::saveMessage(t('Box @key does not exist in destination.  Ignoring box.', array('@key'=>$box->plugin_key)), MigrationBase::MESSAGE_WARNING);
      return FALSE;
    }
    
//    $row->object_id = $object_id;
    $row->value = serialize($value);
    return parent::prepareRow($row);
  }

  function _update_box($box, $id) {
    $BP = new BoxPorter($this);
    $box = $BP->_update_box($box, $id);
    
//     if (isset($box->region)) {
//       unset($box->region);
//     }
    
    return $box;
  }
  
  function _box_key_exists($key) {
    $BP = new BoxPorter($this);
    return $BP->_box_key_exists($key);
  }
}

/**
 * @class BoxPorter
 * 
 * Box porting functions need to be avaialble to contexts, but shouldn't be part of the rest of migration.  Lets put them all here to contain them but make them accessible.
 * 
 * @author sagotsky
 *
 */
class BoxPorter {
  
  public function __construct($migration) {
    $this->migration = $migration;
  }
  
  public function _update_block_plugin_key($key) {
  
    $box_map = array(
      'os_modal_tabbed' => 'os_boxes_tabs',
      'os_boxes_modal_sitelogo' => 'os_boxes_sitelogo', //needs pulling from orig vsite
      'os_boxes_modal_siteinfo' => 'os_boxes_siteinfo', //needs pulling from orig vsite
      'vsite_taxonomy_fbt' => 'os_taxonomy_fbt',
      'os_boxes_currtoc' => 'os_boxes_booktoc', //needs an option added
    );
  
    
    //try mapping the key
    if (isset($box_map[$key])) {
      return $box_map[$key];
    } 

    //check if we have a function for it
    if ($newkey = $this->_port_box($key, 0, TRUE)) {
      return $newkey;
    }
  
    //check if the key we're using exists or if we're ignoring it.
    if (module_exists('os_boxes')) {
      $boxes = os_boxes_get_boxes_plugins();
      if (isset($boxes[$key])) {
        return $key;
      } else {
        return FALSE;
      }
    } else {
      return $key;
    }
  
  }
  
  public function _box_key_exists($plugin_key) {
    if (module_exists('os_boxes')) {
      $boxes = os_boxes_get_boxes_plugins();
      return isset($boxes[$plugin_key]);
    }
  }
  
  //refactored _update_block that also updates the box.  
  public function _update_box($box, $id) {

    if (isset($box->region) && ($newregion = $this->migration->_update_region($box->region))) {
      $box->region = $newregion;
    } 
    
    //matt says plugins should always have boxes as a module.
    if (isset($box->module)) {
      $box->module = $this->migration->_update_module($box->module);
    }
    
    /* three kinds of updates
     no change
    key changes
    key,box change
    box change
    delete
    */
    
    //first try box, box/key change.  if a function does this it should be complete and we can exit.
    $new = $this->_port_box($box, $id);
    if (isset($new)) {
      return $new; //set or false, it returns. 
    } 
    
    //key check.  either map a new key, or confirm that this key exists.
    if ($key = $this->_update_block_plugin_key($box->plugin_key)) {
      $box->plugin_key = $key;
      return $box;
    }
    
    return FALSE;
  }
  
  
  //wrapper function around the _port_box_$PLUGIN functions
  //key only is used for getting the key of the box without actually porting it.  seems easier than maintaining an array
  public function _port_box($box, $id, $key_only = FALSE) {
    if (is_string($box) && $key_only) {
      $box = (object) array('plugin_key' => $box); //make a box object for the purpose of fetching a new key.
    }
    
    $port = '_port_box_' . $box->plugin_key;
    if (method_exists($this, $port)) {
      return $this->$port($box, $id, $key_only);
    } 
    
    return;
  }
  
  public function _port_box_os_boxes_modal_image($box, $id, $key_only) {
    if ($key_only) {
      return 'os_boxes_html';
    }
    
    //get fid of new entity from migrate's status tables
    $fid = db_select('migrate_map_modalimagenodeos', 'm')
      ->condition('sourceid1', $box->options['nid'])
      ->fields('m', array('destid1'))
      ->execute()
      ->fetchCol();
    $fid = current($fid);
    
    //get crop data from legacy db
    $image_info = $this->migration->d6_db_select('content_type_modal_image', 'ct')
      ->condition('nid', $box->options['nid'])
      ->fields('ct', array('field_modal_image_data', 'field_link_from_image_url', 'field_modal_image_size_value'))
      ->execute()
      ->fetchAssoc();

    if ($fid && ($file = file_load($fid))) {      
      //migrate_map_modalimagenodeos.  sourceid1 = d6 nid.  destid1 = media id.
      
      $image_vars = array(
        'path' => $file->uri,
        //'width',
        //'height',
        'alt' => $box->description,
        'title' => $box->title,
        'attributes' => array(),
      );
      
      $img_html = theme_image($image_vars);
      if (isset($image_info['field_link_from_image_url'])) {
        $img_html = l($img_html, $image_info['field_link_from_image_url'], array('html' => TRUE));  
      }
      
      //modal image should have a separate content type migration.  lets use that for moving the image.
      $box->plugin_key = 'os_boxes_html';
      $box->options = array('text' => array(
        'format' => 'filtered_html',
        'value' => $img_html, 
      ));
      
      return $box;
    } else {
      //in most cases of a modal_image node that didn't import, there was no group owner.
      if ($fid === FALSE) {
        $fid = 'FALSE'; 
      }
      
      //OSMigration::saveMessage
      $this->migration->saveMessage(t('Could not port os_boxes_modal_image.  No file with fid @fid.  Check legacy node: @nid', 
        array('@nid' => $box->options['nid'], '@fid' => $fid)), MigrationBase::MESSAGE_WARNING);
      return FALSE;
    }
  }
  
  // public function _port_box_os_boxes_modal_siteinfo($box, $key_only) {
  // $key = 'os_boxes_site_info';
  // if ($key_only) {
  // return $key;
  // }
  
  
  // //title
  // $query = parent::d6_db_select('node', 'n')
  // ->condition('type', 'vsite')
  // ->condition('nid', $nid)
  // ->fields('n', array('title'));
  
  // //address
  // $query->join('content_type_vsite', 'ct', 'n.nid = ct.nid');
  // $query->fields('ct', array('field_vsite_address_value', 'field_vsite_address_format'));
  
  // //description
  // $query->join('og', 'og', 'n.nid = og.nid');
  // $query->fields('og', array('og_description'));
  
  // if ($result = $query->execute()->fetchAssoc()) {
  // $box->plugin_key = $key;
  
  // $box->options = array(
  // 'site_title' => $result['title'],
  // 'descrip' => $result['og_descrioption'],
  // 'address1' => $result['field_vsite_address_value'],
  // );
  
  // return $box;
  // } else {
  // return FALSE;
  // }
  
  
  // }
  
  // public function _port_box_os_boxes_curr_toc($box, $key_only) {
  // $key = 'os_boxes_booktoc';
  // if ($key_only) {
  // return $key;
  // }
  
  // }
  
  //port html box
  public function _port_box_os_boxes_simple($box, $id, $key_only) {
    $key = 'os_boxes_html';
    if ($key_only) {
      return $key;
    }
  
    $body = $box->options['body'];
    $format = (isset($box->options['format']) && in_array($box->options['format'], array(1,2,4,5,6))) ? 'filtered_html' : 'plain_text'; //filtered, full, comment, minimal, full -> filtered.
  
    unset($box->options['body'], $box->options['format']);
  
    $box->options = array('text' => array(
      'value' => $body,
      'format' => $format,
    ));
    $box->plugin_key = $key;
  
    return $box;
  }
  
  //attach a style option
  public function _port_box_os_boxes_manual_list($box, $id, $key_only) {
    if ($key_only) {
      return $id;
    }
    
    if (!isset($box->options['style'])) {
      $box->options['style'] = 'title';
    }
    
    return $box;
  }
  
  //'os_boxes_shields' => 'os_shields_box', (box holds the filename.  used to be in a var.)
  public function _port_box_os_shields_box($box, $id, $key_only) {
    $key = 'os_shields_box';
    if ($key_only) {
      return $key;
    }
    
    //this depends on variable having already been imported.
    $value = db_select('spaces_overrides', 'so')
      ->condition('object_type', 'variable')
      ->condition('object_id', 'os_shields_shield') //name has already been transformed...
      ->condition('id', $id)
      ->condition('type', 'og')
      ->fields('so', array('value'))
      ->execute()
      ->fetchCol();
    
    if ($shield_path = unserialize($value)) {
      $shield_file = end(explode('/', $shield_path));
      $box->options['shield'] = $shield_file;
    } else {
      $box->options['shield'] = '';
    }
    $box->plugin_key = $key;
    
    return $box;
  }
  
  //twitter prefix is separated into own variable
  public function _port_box_os_boxes_twitterfeed($box, $id, $key_only) {
    if ($key_only) {
      return 'os_boxes_twitterfeed';
    }
  
    $prefix = $box->options['twitter_username'][0];
    if ($prefix == '@') {
      $box->options['twitter_type'] = 'user';
      $box->options['twitter_username'] = substr($box->options['twitter_username'], 1);
    } elseif ($prefix == '#') {
      $box->options['twitter_type'] = 'hashtag';
      $box->options['twitter_username'] = substr($box->options['twitter_username'], 1);
    } else {
      $box->options['twitter_type'] = 'search';
    }
  
    return $box;
  }
  
  public function _port_box_os_boxes_modal_sitelogo($box, $id, $key_only) {
    $key = 'os_boxes_site_logo';
    if ($key_only) {
      return $key;
    }
  
    if (($vsite = node_load($id)) == FALSE) {
      return FALSE;
    }
  
    if (isset($vsite->field_site_logo[LANGUAGE_NONE]) && $logo = current($vsite->field_site_logo[LANGUAGE_NONE])) {
      $box->options = array(
        'fid' => $logo['fid'],
        'x' => $logo['cropbox_x'],
        'y' => $logo['cropbox_y'],
        'height' => $logo['cropbox_height'],
        'width' => $logo['cropbox_width'],
      );
      $box->plugin_key = $key;
  
      return $box;
    } else {
      $this->migration->saveMessage(t('Could not migrate site logo. Image does not exist. Vsite nid: @nid', array('@nid' => $id)), MigrationBase::MESSAGE_WARNING);
      return FALSE;
    }
  }
  
  public function _port_box_os_boxes_gallery($box, $id, $key_only) {
    $key = 'os_boxes_slideshow';
    if ($key_only) {
      return $key;
    }

    $box->options += array(
      'controls' => 'none',
      'slide_counter' => 'none',
      'pager' => 'none',
      'effect' => 'fade',
      'style' => 'dark',
      'size' => 600,
    );
   
    $box->options['gallery_nid'] = $box->options['gallery'];
    unset($box->options['gallery']);
    
    $box->options['items_per_page'] = $box->options['number_of_items'];
    unset($box->options['number_of_items']);
    
    $box->plugin_key = $key;
    
    return $box;
  }
  
  //convert os_boxes_node to custom html.  102 in db
  public function _port_box_os_boxes_node($box, $id,  $key_only) {
    if ($key_only) {
      return 'os_boxes_html';
    }
    
    $body = '';
    
    $nid = $box->options['nid'];
    $teaser = $box->options['teaser'];
    $show_title = $box->options['show_title'];
    
    //some nids include the title.  (ie "this is my post[nid:1234]")
    if ((!(int)$nid) && strpos($nid, ':')) {
      list(,$nid) = explode(':', $nid);
      $nid = (int) $nid;
    }
    
    if (!$nid) {
      $this->migration->saveMessage(t('Could not port os_boxes_node.  Missing nid.'), MigrationBase::MESSAGE_WARNING);
      return FALSE; //can't port this without a nid.
    }
    
    $query = $this->migration->d6_db_select('node', 'n')
      ->fields('n', array('nid'))
      ->condition('n.nid', $nid);
    $query->join('node_revisions', 'nr', 'n.vid = nr.vid');
    $query->fields('nr', array('title', 'body', 'teaser', 'format'));
    $result = $query->execute();
    
    if ($result->rowCount()) {
      $node = $result->fetchObject();
      
      if ($show_title) {
        $body .= $node->title;
      }
      
      if ($teaser = 'body') {
        $body .= $node->body;
      } else {
        $body .= $node->teaser;
      }
      
      $box->plugin_key = 'os_boxes_html';
      $box->options = array('text'=>array(
        'value' => $body,
        'format' => ($node->format == 1) ? 'filtered_html' : 'plain_text',  
      ));
      
      return $box;
    } else {
      $this->migration->saveMessage(t('Could not port os_boxes_node.  No node with nid @nid.', array('@nid' => $nid)), MigrationBase::MESSAGE_WARNING);
      return FALSE;
    }
  }
}
