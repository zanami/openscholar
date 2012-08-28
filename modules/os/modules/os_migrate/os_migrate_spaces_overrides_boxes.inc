<?php 

class SpacesOverridesBoxesMigration extends SpacesOverridesTableMigration {
  
  public function __construct() {
    $this->source_object = $this->dest_object = 'boxes';
    $this->dependencies[] = 'ModalImageNodeOS'; //images have to be copied before modal boxes can migrate.
    parent::__construct();
  }
  
  public function query() {
    $query = parent::query();
    $query->condition('object_id', '', '!=');
    
    
    //$query->condition('value', '%os_boxes_node%', 'LIKE'); //testing the port function
    $query->condition('value', '%os_boxes_simple%', 'LIKE'); //testing the port function
    
    
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
    
    
//     if (($value = $this->_update_block((array)$value)) === FALSE) {
//       return FALSE;
//     } else {
//       $value = (object)$value;
//     }
    if ($box = $this->_update_box($value)) {
      $value = $box;
    } else {
      return FALSE;
    }
    
    
//    $row->object_id = $object_id;
    $row->value = serialize($value);
    return parent::prepareRow($row);
  }
  
  
  //refactored _update_block that also updates the box.  
  public function _update_box($box) {

    if (isset($box->region)) {
      $box->region = $this->_update_region($box->region);
    }
    
    if (isset($box->module)) {
      $box->module = $this->_update_module($box->module);
    }
    
    /* three kinds of updates
     no change
    key changes
    key,box change
    box change
    delete
    */
    
    //first try box, box/key change.  if a function does this it should be complete and we can exit.
    $new = $this->_port_box($box);
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
  public function _port_box($box, $key_only = FALSE) {
    $port = '_port_box_' . $box->plugin_key;
    if (method_exists($this, $port)) {
      return $this->$port($box, $key_only);
    } 
    
    return;
  }
  
  public function _port_box_os_boxes_modal_image($box, $key_only) {
    if ($key_only) {
      return 'simple';
    }
    
    //get fid of new entity from migrate's status tables
    $fid = db_select('migrate_map_modalimagenodeos', 'm')
      ->condition('sourceid1', $box->options['nid'])
      ->fields('m', array('destid1'))
      ->execute()
      ->fetchCol();
    $fid = current($fid);
    
    //get crop data from legacy db
    $image_info = parent::d6_db_select('content_type_modal_image', 'ct')
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
      $box->plugin_key = 'simple';
      $box->options = array(
        'format' => 'filtered_html',
        'body' => $img_html, 
      );
      
      return $box;
    } else {
      //in most cases of a modal_image node that didn't import, there was no group owner.
      //but psaces_overrides knows the group.  acquire from there?  maybe modal_image could do a prepare query for one type then the other 
      if ($fid === FALSE) {
        $fid = 'FALSE'; 
      }
      
      parent::saveMessage(t('Could not port os_boxes_modal_image.  No file with fid @fid.  Check legacy node: @nid', 
        array('@nid' => $box->options['nid'], '@fid' => $fid)), MigrationBase::MESSAGE_WARNING);
      return FALSE;
    }
  }
  
  //port sitelogo
//   public function _port_box_os_boxes_modal_sitelogo($box, $key_only) {

//   }
  
//   public function _port_box_os_boxes_modal_siteinfo($box, $key_only) {
//     $key = 'os_boxes_site_info';
//     if ($key_only) {
//       return $key;
//     }
    
    
//     //title
//     $query = parent::d6_db_select('node', 'n')
//       ->condition('type', 'vsite')
//       ->condition('nid', $nid)
//       ->fields('n', array('title')); 
    
//     //address
//     $query->join('content_type_vsite', 'ct', 'n.nid = ct.nid');
//     $query->fields('ct', array('field_vsite_address_value', 'field_vsite_address_format'));
    
//     //description
//     $query->join('og', 'og', 'n.nid = og.nid');
//     $query->fields('og', array('og_description'));
    
//     if ($result = $query->execute()->fetchAssoc()) {
//       $box->plugin_key = $key;
      
//       $box->options = array(
//         'site_title' => $result['title'],
//         'descrip' => $result['og_descrioption'],
//         'address1' => $result['field_vsite_address_value'],
//       );
      
//       return $box;
//     } else {
//       return FALSE;
//     }
    
    
//   }
  
//   public function _port_box_os_boxes_curr_toc($box, $key_only) {
//     $key = 'os_boxes_booktoc';
//     if ($key_only) {
//       return $key;
//     }
    
//   }
  
  //port html box
  public function _port_box_os_boxes_simple($box, $key_only) {
    $key = 'os_boxes_html';
    if ($key_only) {
      return $key;
    }
    
    $body = $box->options['body'];
    $format = (isset($box->options['format']) && $box->options['format'] == 1) ? 'filtered_html' : 'plain_text';
    
    unset($box->options['body'], $box->options['format']);
    
    $box->options = array(
      'value' => $body,
      'format' => $format,
    );
    $box->plugin_key = $key;
    
    return $box;
  }
  
  //convert os_boxes_node to custom html.  102 in db
  public function _port_box_os_boxes_node($box, $key_only) {
    if ($key_only) {
      return 'simple';
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
      parent::saveMessage(t('Could not port os_boxes_node.  Missing nid.'), MigrationBase::MESSAGE_WARNING);
      return FALSE; //can't port this without a nid.
    }
    
    $query = parent::d6_db_select('node', 'n')
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
      
      $box->plugin_key = 'simple';
      $box->options = array(
        'value' => $body,
        'format' => ($node->format == 1) ? 'filtered_html' : 'plain_text',  
      );
      
      return $box;
    } else {
      parent::saveMessage(t('Could not port os_boxes_node.  No node with nid @nid.', array('@nid' => $nid)), MigrationBase::MESSAGE_WARNING);
      return FALSE;
    }
  }
}
