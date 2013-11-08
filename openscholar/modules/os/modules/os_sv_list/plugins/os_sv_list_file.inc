<?php 

class os_sv_list_file extends os_sv_list {
  public function __construct() {
    parent::__construct();
    $this->entity_type = 'file';
  }
  /**
   * Implementation of boxes_content::options_defaults().
   */
  /*
  public function options_defaults() {
    $options = parent::options_defaults();
    return $options;
  }
  */
  
  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);
    $defaults = $this->options_defaults();
    $form['file'] = array('#markup' => '<h3>This box is for files</h3>');
    $types = array('all' => t('All')) + 
      array_reduce(file_type_get_all_types(), function ($ret, $o) {$ret[$o->type] = $o->label; return $ret;});
    $form['content_type']['#title'] = t('File Type');
    $form['content_type']['#options'] = $types;
    return $form;
  }
  
  public function render() {
    $block = parent::render();
    
    //$block['content'] .= ' files';
    return $block;
  }

  protected function _display_options() {
    return array(
      'display_title' => t('Title'),
      'display_preview' => t('Preview'),
      'display_details' => t('Details'),
      'display_grid' => t('Grid'),
    );
  }
  
  protected function _sort_options() {
    return array(
      'sort_newest' => t('Newest'),
      'sort_oldest' => t('Oldest'),
      'sort_alpha' => t('Alphabetical'),
      'sort_filesize' => t('Filesize'),
      //usage?
    );
  }
  
  /**********************
   * Display functions  *
   **********************/
//   function display_title($ids) {
//     return $this->display_view_mode($ids, ''); //title view mode?
//   }
  function display_preview($ids) {
    return $this->display_view_mode($ids, 'preview');
  }
  function display_details($ids) {
    return $this->display_view_mode($ids, 'full');
  }
  function display_grid($ids) {
    return $this->display_view_mode($ids, '');  //todo grid view
  }
  
  /*
   * 
   file views
     //$entity_info = entity_get_info("file"); print_r(array_keys($entity_info["view modes"]));
  Array
  (
      [0] => ical
      [1] => diff_standard
      [2] => teaser
      [3] => full
      [4] => preview
      [5] => search_index
      [6] => search_result
      [7] => media_gallery_thumbnail
      [8] => media_gallery_lightbox
      [9] => media_gallery_detail
      [10] => media_gallery_block_thumbnail
      [11] => os_files_xxlarge
      [12] => os_files_xlarge
      [13] => os_files_large
      [14] => os_files_med
      [15] => os_files_small
      [16] => os_files_link_icon
      [17] => os_files_link
      [18] => token
  )*/
  
}