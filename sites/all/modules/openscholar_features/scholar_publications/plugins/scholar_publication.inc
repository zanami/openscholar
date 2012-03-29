<?php 

/**
 * @class scholar_publication
 * 
 * Extends os_simpleview to provide options for publication
 */

class scholar_publication extends os_simpleview_default {
  
  //add published date to the sort form
  public function os_simpleview_form_display_options($options = NULL, $step = NULL){
    $form = parent::os_simpleview_form_display_options($options, $step);
    $form['sorts_fieldset']['sorts']['#options'] += array('published_date' => t('Recently Published'));
    return $form;
  }
    
  //override the default view.  this plugin uses different views depending on the group by option
  function view_settings($options = array()) {
    //expose plugin opts like this?  or just take the plugin opts?
    switch($options['plugin_opts']['group_by']) {
      case 'type':
        return (object)array('view_name' => 'publication_types', 'display_id' => 'block_1');
        
      case 'coauthor':
        return (object)array('view_name' => 'publication_contributors', 'display_id' => 'block_1'); 
      
      case 'year': 
        return (object)array('view_name' => 'publication_years', 'display_id' => 'block_1');
    
      case 'none':
      default: 
        return (object)array('view_name' => 'publication_types', 'display_id' => 'block_3');
    }    
  }
  
  //add defaults for the new form options
  function options_defaults() {
    $options = parent::options_defaults(); //maybe this should just be plugin opts and ignore the rest of the defaults?
    
    $options['sorts'] = 'published_date';    
    $options['number_of_items'] = 10;
    $options['use_pager'] = TRUE;
    $options['plugin_opts']['show_all_types'] = TRUE;
    $options['plugin_opts']['pub_types'] = array();
    $options['plugin_opts']['group_by'] = 'none';
    $options['plugin_opts']['display_num'] = TRUE;
    return $options;
  }
  
  // add new form
  function multiform_info() {    
    $form = parent::multiform_info();
    $form['publication_options'] = array(
      'form id'	=> 'os_simpleview_scholar_publication_options',
      'title' => t('Publication Options'),
    );
    return $form;
  }
  
  //defines new form.  adds type selection, group by select
  // need to communicate that group by and sort by are exclusive.  show groups/show pubs with dependent selects?
  function os_simpleview_scholar_publication_options($options = NULL) {
    $q = db_query("SELECT tid, name FROM {biblio_types} WHERE tid > 0 AND visible = 1 ORDER BY name ASC");
    $pub_types = array();
    while ($a = db_fetch_array($q)) {
      $pub_types[$a['tid']] = $a['name'];
    }
    
    $form = array(
			'show_all_types' => array(
        '#type' => 'checkbox',
        '#title' => t('Show all publication types'),
        '#default_value' => $options['show_all_types'],
        '#tree' => TRUE,
        '#description' => t('Include all your publications or restrict based on type.'),
      ),
    
			'pub_types' => array(
        '#type' => 'checkboxes',
        '#options' => $pub_types,
        '#default_value' => $options['pub_types'],
        '#title' => t('Show the following types'),
        '#attributes' => array('class' => 'pub_type'),
        '#prefix' => '<div id="edit-pub-types-wrapper">',
        '#suffix' => '</div>',
        '#process' => array(
          'ctools_dependent_process','expand_checkboxes'
        ),
        '#dependency' => array(
          'edit-show-all-types' => array(
            0
          )
        ),
      ),
      
      //does grouping obviate sorting?
      'group_by' => array(
        '#type' => 'select',
        '#title' => t('Group By'),
        '#options' => array('none'=>t('None'), 'year' => t('Year'), 'coauthor'=>t('Co-author'), 'type'=>t('Type')),
        '#default_value' => $options['group_by'],
        '#description' => t('Instead of sorting your publications, show links to your publications grouped by year, co-author, or type.')
      ),
      
      'display_num' => array(
        '#type' => 'checkbox',
        '#title' => 'Display number of items',
        '#default_value' => $options['display_num'],
        '#description' => t('When displaying groups of publications, show the total number of publications in each group.')
      ),
    );
    
    return $form;
  }
  
  // append a filter to only show selected biblio types
  function view_append($opts) {
    $append = parent::view_append($opts);
    if (!$opts['show_all_types'] && count($opts['pub_types']) ) {
      $append['filters'] += array(
        'biblio_type' => array(
        'operator' => 'in',
        'value' => $opts['pub_types'],
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array('operator' => FALSE, 'label' => ''),
        'id' => 'tid',
        'table' => 'biblio_types',
        'field' => 'tid',
        'override' => array('button' => 'Use default'),
        'relationship' => 'none',
        ),
      );
    }
    return $append;
  }
  
  //default views_alter can't handle views arguments, so let's alter the view
  public function views_alter(view &$view, &$display_id, &$args, $options) {
    
    parent::views_alter($view, $display_id, $args, $options);
  
    if (isset($options['display_num'])) {
      switch ($options['group_by']) {
        case 'coauthor': $arg = 'lastname'; break;
        case 'year': $arg = 'biblio_year'; break;
        case 'type': $arg = 'name'; break;
        default: $arg = NULL;
      }
      
      $arguments = $view->display_handler->get_option('arguments');
      
      if (isset($arguments[$arg])) {
        $arguments[$arg]['style_options']['count'] = $options['display_num'];
        $view->display_handler->override_option('arguments', $arguments); 
      } 
    }
  }
  
  //override and extend parse_options_sorts to add pub date
  protected  function _parse_options_sorts($sorting_method) {
    $override = array();
    switch ($sorting_method) {
      case 'published_date':
        $override['sorts']['biblio_year'] = array(
          'order' => 'DESC',
          'delta' => -1,
          'id' => 'biblio_year',
          'table' => 'biblio',
          'field' => 'biblio_year',
          'relationship' => 'none',
          'override' => array(
            'button' => 'Override',
          ),
        );
      break;
       
      default:
        $override = parent::_parse_options_sorts($sorting_method);
      break;
        
    }
    return $override;
  }
  
  //view is not field based.  make it field based for titles
  protected function _parse_options_row_options($display_style) {
    $override = parent::_parse_options_row_options($display_style);
    if ($display_style == 'titles') {
      $override['row_plugin'] = 'fields';
    }
    return $override;
  }
}