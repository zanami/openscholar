<?php 

class os_sv_list_node extends os_sv_list { 
  public function __construct() {
    parent::__construct();
    $this->entity_type = 'node';
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
    
    $content_types = array(
      'all' => t('All'),
    ) + os_get_bundles(array(1));
    
    if (isset($content_types['media_gallery'])) {
      $content_types['image'] = t('Images');
    }
    

    //$content_types_value = !empty($form_state['values']['content_type']) ? $form_state['values']['content_type'] : $this->options['content_type'];
    
  //  $form['content_type']['#default_value'] = $content_types_value;
    $form['content_type']['#options'] = $content_types;

    
    // Defines which display types are content type specific.
    $form['display']['#attached']['js'][] = array('type'=>'setting', 'data'=>array('os_sv_list' => array('has_display' => array(
      'display_sidebar_teaser' => array('news', 'event', 'person'),
      'display_links' => array('link'),
    ))));
    
    return $form;
  }
  
//   public function render() {
//     $block = parent::render();
//     //$block['content'] .= ' posts';
//     return $block;
//   }

  public function query() {
    $efq = parent::query();
    $efq->propertyCondition('status', 1); 
    return $efq;
  }

  //add our node specific sorts
  protected function _sort_options() {
    $sorts = array(
      // Standard sorts,
      'sort_newest' => t('Newest Post'),
      'sort_oldest' => t('Oldest Post'),
      'sort_alpha' => t('Alphabetical'),
      'sort_random' => t('Random'),
      
     
      // Content type sensitive sorts.  // in the future get these from sv render plugins
//       'sort_event' => t('Event Date'),
//       'sort_presentation' => t('Recently Presented'),
//       'sort_biblio' => t('Year of Publication'),
//       'sort_news' => t('News Date'),
  
      
    );
    
    
    return parent::_sort_options() + $sorts;
  }
    
    
  /**
   * sorting functions
   */
  function sort_newest(&$efq) {
    //could thiese be made generic by using entity_get_info to figure out $created and $title per type?
    $efq->propertyOrderBy('created', 'DESC');
  }
  function sort_oldest(&$efq) {
    $efq->propertyOrderBy('created', 'ASC');
  }
  function sort_alpha(&$efq) {
    $efq->propertyOrderBy('title', 'ASC');
  }
  
}