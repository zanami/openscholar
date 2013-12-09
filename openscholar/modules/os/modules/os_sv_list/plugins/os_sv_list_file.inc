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
    $types = array('all' => t('All')) + 
      array_reduce(file_type_get_all_types(), function ($ret, $o) {$ret[$o->type] = $o->label; return $ret;});
    $form['content']['content_type']['#title'] = t('File Type');
    $form['content']['content_type']['#options'] = $types;
    return $form;
  }
  
  public function render() {
    $block = parent::render();
    
    //$block['content'] .= ' files';
    return $block;
  }

//   protected function _display_options() {
//     $info = entity_get_info('file'); 
//     foreach ($info['view modes'] as $mode => $info) {
//       $modes[$mode] = $info['label'];
//     }
    
//     dpm($modes);
//     return $modes; //lets see how this plays out....
//     return array(
//       'display_title' => t('Title'),
//       'display_preview' => t('Preview'),
//       'display_details' => t('Details'),
//       'display_grid' => t('Grid'),
//     );
//   }
  
  protected function _sort_options() {
    return array(
      'sort_newest' => t('Newest'),
      'sort_oldest' => t('Oldest'),
      'sort_alpha' => t('Alphabetical'),
      'sort_filesize' => t('Filesize'),
      'sort_random' => t('Random'),
    );
  }
  

  /**
   * sorting functions
   */
  function sort_newest(&$efq) {
    $efq->propertyOrderBy('timestamp', 'DESC');
  }
  function sort_oldest(&$efq) {
    $efq->propertyOrderBy('timestamp', 'ASC');
  }
  function sort_alpha(&$efq) {
    $efq->propertyOrderBy('filename', 'ASC');
  }
  function sort_filesize(&$efq) {
    $efq->propertyOrderBy('filesize', 'DESC');
  }
  
}