<?php 

class SpacesOverridesBoxesMigration extends SpacesOverridesTableMigration {
  
  public function __construct() {
    $this->source_object = $this->dest_object = 'boxes';
    parent::__construct();
  }
  
  public function query() {
    $query = parent::query();
    $query->condition('object_id', '', '!=');
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
    
    if (($value = $this->_update_block((array)$value)) === FALSE) {
      return FALSE;
    } else {
      $value = (object)$value;
    }
    
    
//    $row->object_id = $object_id;
    $row->value = serialize($value);
    return parent::prepareRow($row);
  }
  
}

/*
 * box plugin keys
 * 
fb_social_activityf
fb_social_comments
fb_social_facepile
fb_social_like
fb_social_likebox
fb_social_livestream
fb_social_recommend
fb_social_send
os_boxes_addthis
os_boxes_bio
os_boxes_booktoc
os_boxes_cclicense
os_boxes_curr_toc
os_boxes_feedreader
os_boxes_follow
os_boxes_gallery
os_boxes_manual_list
os_boxes_modal_image
os_boxes_modal_sitelogo
os_boxes_node
os_boxes_oembed
os_boxes_present_list
os_boxes_pub_ca
os_boxes_pub_type
os_boxes_pub_typ_list
os_boxes_pub_year
os_boxes_recent_docs
os_boxes_rss
os_boxes_shields
os_boxes_simple
os_boxes_slideshow
os_boxes_sv_list
os_boxes_twitterfeed
os_modal_tabbed
stdClass
value
vsite_taxonomy_fbt

 */