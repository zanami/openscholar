<?php 

class SpacesOverridesVariableMigration extends SpacesOverridesTableMigration {
  
  public function __construct() {
    $this->source_object = $this->dest_object = 'variable';
    parent::__construct();
  }
  
  public function query() {
    $query = parent::query();
    
    return $query;
  }
  
  public function prepareRow(&$row) {
    if (!$value = unserialize($row->value)) {
      if ($row->value == 'NULL') {
        return; //unless we have to update object_ids.
      }
      $value = $row->value; //serialize them all when we're done.
    }
    $object_id = $row->object_id;
    
    //flavor variables get new names
    if (preg_match('/_flavor$/', $row->object_id)) {
      //$this->_update_flavors($row);
      list($object_id, $value) = $this->_update_flavors($object_id, $value);
    }
   
    /*
     * pseudo-hook:  $this->_update_TYPE_ID($object_id, $value);
    * always returns array of paramters
    */
    $function = '_update_' . $row->object_type . '_' . $object_id;
    if (method_exists($this, $function)) {
      list($object_id, $value) = $this->$function($object_id, $value);
    }
    
    $row->object_id = $object_id;
    $row->value = serialize($value);
    return parent::prepareRow($row);
  }

  /*
   * @function _update_flavors
  *
  * Updates flavor variables and names.
  */
  public function _update_flavors($object_id, $value) {
    $object_id = preg_replace('/vsite_design_scholar/', 'os_appearance', $object_id);
    $value = preg_replace('/^scholar_/', '', $value);
    return array($object_id, $value);
  }
  

  /**
   * pseudo hooks (for lack of a better term)
   *
   * To save a little space and keep from writing a million checks for object id, I'm using the type
   * and id for the name of a variable function.  _update_TYPE_ID.
   *
   * Takes object_id and value.  Returns array of the same to be set with list().
   */
  
  /**
   * Update the spaces_features variable by fixing the module names.
   */
  public function _update_variable_spaces_features($object_id, array $value) {
    $spaces_features = array_flip( $this->_update_module(array_flip($value)) );   //should this rename be a function?
    return array($object_id, $value);
  }
  
  /**
   * spaces_preset_og variable
   */
  public function _update_variable_spaces_preset_og($object_id, $value) {
    $presets = array(
      'scholar' => 'os_personal',
      'project' => 'os_department',
    );
    
    return (isset($presets[$value])) ? $presets['value'] : FALSE;
  }
  
}

/**** all the variables (in d6's spaces_override)****
 
biblio_author_links
biblio_export_links
biblio_google_link
biblio_link_title_url
biblio_no_year_text
biblio_order
biblio_rowsperpage
biblio_sort
biblio_sort_tabs
biblio_style
comment_announcement
comment_anonymous_biblio
comment_anonymous_bio
comment_anonymous_blog
comment_anonymous_book
comment_anonymous_gallery
comment_anonymous_link
comment_anonymous_page
comment_biblio
comment_bio
comment_blog
comment_book
comment_class
comment_class_material
comment_controls_biblio
comment_controls_bio
comment_controls_blog
comment_controls_book
comment_controls_gallery
comment_controls_link
comment_controls_page
comment_cv
comment_default_mode_biblio
comment_default_mode_bio
comment_default_mode_blog
comment_default_mode_book
comment_default_mode_gallery
comment_default_mode_link
comment_default_mode_page
comment_default_order_biblio
comment_default_order_bio
comment_default_order_blog
comment_default_order_book
comment_default_order_gallery
comment_default_order_link
comment_default_order_page
comment_default_per_page_biblio
comment_default_per_page_bio
comment_default_per_page_blog
comment_default_per_page_book
comment_default_per_page_gallery
comment_default_per_page_link
comment_default_per_page_page
comment_event
comment_feed
comment_form_location_biblio
comment_form_location_bio
comment_form_location_blog
comment_form_location_book
comment_form_location_gallery
comment_form_location_link
comment_form_location_page
comment_gallery
comment_image
comment_link
comment_page
comment_person
comment_presentation
comment_preview_biblio
comment_preview_bio
comment_preview_blog
comment_preview_book
comment_preview_gallery
comment_preview_link
comment_preview_page
comment_subject_field_biblio
comment_subject_field_bio
comment_subject_field_blog
comment_subject_field_book
comment_subject_field_gallery
comment_subject_field_link
comment_subject_field_page
cp_admin_menu
disqus_domain
disqus_nodetypes
facebook_comments_nodetypes
os_mailchimp_api
rowsperpage
scholar_biocv_use_pdf_as_cv
scholar_book_toc_location
scholar_dvn_dvn_alias
scholar_front_create_bio
scholar_front_feature_nodes
scholar_front_frontpage_nid
scholar_front_frontpage_path
scholar_front_hand_pick_help
scholar_front_show_title
scholar_front_site_title
scholar_publications_external_link_name
scholar_publications_filter_publication_types
scholar_publications_note_in_teaser
scholar_publications_shorten_citations
scholar_publication_note_in_teaser
scholar_reader_twitter_username
site_frontpage
spaces_features
spaces_preset_og
space_menu_items
vsite_activity_insert
vsite_activity_update
vsite_comments_comment_type_announcement
vsite_comments_comment_type_biblio
vsite_comments_comment_type_bio
vsite_comments_comment_type_blog
vsite_comments_comment_type_book
vsite_comments_comment_type_class
vsite_comments_comment_type_class_material
vsite_comments_comment_type_cv
vsite_comments_comment_type_event
vsite_comments_comment_type_feed
vsite_comments_comment_type_gallery
vsite_comments_comment_type_image
vsite_comments_comment_type_link
vsite_comments_comment_type_page
vsite_comments_comment_type_person
vsite_comments_comment_type_presentation
vsite_design_disable_shield
vsite_design_maintheme
vsite_design_scholar_aglet_flavor
vsite_design_scholar_airstream_flavor
vsite_design_scholar_bigpic_flavor
vsite_design_scholar_cleanblue_flavor
vsite_design_scholar_collector_flavor
vsite_design_scholar_collegiate_flavor
vsite_design_scholar_density_flavor
vsite_design_scholar_eloquent_flavor
vsite_design_scholar_quinn_flavor
vsite_design_scholar_redhead_flavor
vsite_design_scholar_stripy_flavor
vsite_design_scholar_weft_flavor
vsite_ga_google_analytics_id
vsite_news_last_access
vsite_private
vsite_setting_contact
vsite_setting_generic
vsite_setting_messages_confirmed
vsite_setting_site
vsite_shields_disable_shield
vsite_shields_shield
vsite_users_contact_form_anonymous

 */

