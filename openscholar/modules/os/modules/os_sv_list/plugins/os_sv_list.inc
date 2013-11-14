<?php

class os_sv_list extends os_boxes_default {
  
  private $plugins = array();
  protected $entity_type;
  protected $bundle;
  
  public function __construct() {
    $this->entity_type = $this->bundle = NULL;
  }
  
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();

    $options += array(
      'content_type' => 'all',
      'display' => 'teaser',
      'more' => FALSE,
      'more_text' => t('More'),
      'more_link' => '', // what's a good default?
  
      'pager' => FALSE,
      'number_of_items' => 6,
      'sort_by' => 'sort_newest',
      'vocabs' => array(),
      'terms' => array(),
      'only_vocabs' => TRUE,
      'only_vocabs_values' => array(),

    );

    //get options from plugins
    if (empty($this->plugins)) {
      $this->_set_plugins($this->entity_type);
    }
    
    foreach ($this->plugins as $plugin) {
      if (method_exists($plugin['instance'], 'options_defaults')) {
        $options += $plugin['instance']->options_defaults();
      }
    }
    
  
    return $options;
  }
  
  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);
    $this->options = ($this->options) ? $this->options + $this->options_defaults() : $this->options_defaults(); //avoid notices when new options are added
    drupal_add_js(array('os_sv_list_box' => array('new_box' => $this->new)), 'setting'); //why am I here? @TODO
  
    $form['#submit'] = array('os_sv_list_form_submit');

    $form['content_type'] = array(
      '#type' => 'select',
      '#title' => t('Content Type'),
//       '#default_value' => $content_types_value,
//       '#options' => $content_types,
      '#id' => 'os_sv_list_content_type',
      '#attached' => array(
        'js' => array(
          drupal_get_path('module', 'os_sv_list').'/os_sv_list.js',
          array('type' => 'setting', 'data' => array('entity_view_modes' => $this->entity_view_modes())),
        ),
      ),
      '#ajax' => array(
        'callback' => 'os_sv_list_vocabs_content_type',
        'wrapper' => 'vocabs',
      ),
    );
    
    $form['display'] = array(
      '#type' => 'select',
      '#title' => 'Display style',
      '#default_value' => $this->options['display'],
      '#options' => $this->_display_options(),
      // Subsite content can only show titles, so hide this element when subsites are active
      '#states' => array(
        'visible' => array(// action to take.
          ':input[name="query_scope[subsites]"]' => array('checked' => FALSE),
        ),
      ),
    );

    $form['sort_by'] = array(
      '#type' => 'select',
      '#title' => 'Sorting',
      '#default_value' => $this->options['sort_by'],
      '#options' => $this->_sort_options(),
    );
  
    $form['query_details'] = array(
      '#prefix' => '<div id="query-details">',
      '#suffix' => '</div>',
      '#parents' => array('query_details'),
    );
  
    $form['#attached'] = array(
      'js' => array(libraries_get_path('select2') . '/select2.js'),
      'css' => array(
        libraries_get_path('select2') . '/select2.css',
        drupal_get_path('module', 'os_sv_list') . '/os_sv_list.css',
      ),
    );


    $form['query_details']['more'] = array(
      '#title' => 'Show "more" link',
      '#type' => 'checkbox',
      '#default_value' => $this->options['more'],
    );
  
    
    //more link.  still needs some refactoring love.
    //populate list of content types => paths
    ctools_include('os', 'os');
    $content_types = array('all' => t('All')) + os_get_bundles(array(1));
    $content_types_value = !empty($form_state['values']['content_type']) ? $form_state['values']['content_type'] : $this->options['content_type']; //wtf
    
    $content_map = os_get_app_path();
    $content_map = array_diff_key($content_map, array_flip(array('bio', 'cv', 'class_material')));
    if (isset($content_types['image'])) {
      $content_map['image'] = $content_map['media_gallery'];
    }
  
    // Use blank path for unknown paths.
    $content_map += array_fill_keys(array_keys(array_diff_key($content_types, $content_map)),' ');
    
    if (!empty($content_map[$content_types_value]) && (empty($this->options['more_link']) || in_array($this->options['more_link'], $content_map))) {
      $default_value = $content_map[$content_types_value];
    } else {
      $default_value = $this->options['more_link'];
    }
  
    if (!empty($form_state['values']['more_link'])) {
    // Unset the values from the text field, as otherwise FAPI will add
    // it to the #value of the form-element.
    // @see http://drupal.org/node/1024962#comment-4021826
        unset($form_state['input']['more_link']);
    }
    
    $form['query_details']['more_link'] = array(
      '#title' => t('URL for the more link'),
      '#type' => 'textfield',
      '#default_value' => $default_value,
      '#states' => array(
        'visible' => array('input[name=more]' => array('checked' => TRUE)),
        'invisible'  => array('input[name=more]' => array('checked' => FALSE)),
      ),
      '#prefix' => '<div id="more_link_div">',
      '#suffix' => '</div>',
      '#attached' => array(
        'js' => array(
          array('data' => array('more_link_defaults' => $content_map,),'type' => 'setting',)
        )
      )
    );
    
    $form['query_details']['more_text'] = array(
      '#title' => 'Label of the more link (default: "more")',
      '#type' => 'textfield',
      '#default_value' => $this->options['more_text'],
        '#states' => array(
          'visible' => array('input[name=more]' => array('checked' => TRUE)),
        ),
    );
    
    $form['pager'] = array(
      '#title' => 'Show pager',
      '#type' => 'checkbox',
      '#default_value' => $this->options['pager'],
    );
  
    $form['number_of_items'] = array(
      '#title' => 'Number of items to display',
      '#type' => 'select',
      '#options' => array_slice(range(0, 20), 1, NULL, TRUE),
      '#default_value' => $this->options['number_of_items'],
    );
    
    //get options from plugins
    foreach ($this->plugins as $plugin) {
      if (method_exists($plugin['instance'], 'options_form')) {
        if ($options_form = $plugin['instance']->options_form($this->options)) {
        
        //add states to only show when this plugin's bundle is selected
        
        //hold off on the states for now.  
        //see https://api.drupal.org/comment/24708#comment-24708 for lists of bundles.  can we do that on nodes or files?
//         foreach ($plugin['bundle'] as $bundle) {
//           $states['visible']['#os_sv_list_content_type'][] = array('value' => $bundle);
//           $states['invisible']['#os_sv_list_content_type'][] = array('!value' => $bundle);
//         }
        
//         if (isset($states)) {
//           foreach ($options_form as $delta => $value) {
//             if (empty($options_form[$delta]['#states'])) {
//               $options_form[$delta]['#states'] = $states;
//             }
//           }     
//         }
          $form += $options_form;
        }
      }
    }

    return $form;
  }
    
  public function render() {
    if (module_exists('vsite') && !vsite_get_vsite()) {
      return;
    }
    
    $bundle = ($this->options['content_type'] != 'all') ? $this->options['content_type'] : NULL;
    $this->_set_plugins($this->entity_type, $bundle);
    
    $block = parent::render();
    $this->options += $this->options_defaults();
    $page_num = (isset($_GET['page'])) ? $_GET['page'] : 0;
    $page = 0;
    $ids = $this->get_ids($page_num, $page);
   
    //add the pager
    if ($this->options['pager'] && $ids) {
      $parameters = array(
        'sv_list_box_delta' => $this->delta,
        'pager_id' => $this->pager_element,
        'destination' => $_GET['q'],
      );
      $use_pager = 'views_mini_pager';
      $pager = theme($use_pager , array(
        'parameters' => $parameters,
        'element' => $this->pager_element,
        'quantity' => $this->options['number_of_items'],
      ));
      
      //if the pager is active, the more link needs its vars to know whether or not to display.
      global $pager_page_array, $pager_total;
      $pager_current = $pager_page_array[$parameters['pager_id']] + 1;
      $pager_max = $pager_total[$parameters['pager_id']];
    } else {
      $pager = '';
    }
        
    //truncate items and prepare the more link
    $more = '';
    if (count($ids) > $this->options['number_of_items'] || ($this->options['pager'] && $ids && $pager_current < $pager_max)) {
      $ids = array_slice($ids, 0, $this->options['number_of_items']);
      $more = ($this->options['more']) ? l($this->options['more_text'], $this->options['more_link'], array('attributes' => array('class' => array('more-link')))) : '';
    }
    
    
    //use drupal display functions or sv list ones.  it'd be great if all these could come from drupal.
    $info = entity_get_info($this->entity_type);
    if (isset($info['view modes'][$this->options['display']])) {
      $entities = entity_load($this->entity_type, $ids);
      $render = entity_view($this->entity_type, $entities, $this->options['display']);
      $content = drupal_render($render);      
    } else {
      $content = 'fix my view mode: ' . $this->options['display'];
    }
    
    $block['content'] = '<div id="box-' . $this->delta . '-page" data-page="'.$page_num.'" data-delta="'.$this->delta.'">' . $content . '</div>' . $pager . $more;
    
    
    return $block;
  }
  
  public function get_ids($page_num, $page) {    
    //////nids
    //EFQ
    //content type
    //   biblio done
    //   image - todo, convert to file box
    //   event - what's with the delta
    //    book list - done
    //    vsite or subsites - done
    $query = $this->query();
    $this->_plugins_invoke('query_alter', $query);
    
    if ($this->options['content_type'] != 'all') {
      $query->propertyCondition('type', $this->options['content_type']);
      //any special hook here?
    }
    
    //more link
    
    //////pager
    if ($this->options['pager'] && is_int($page)) {
      $query->pager($this->options['number_of_items']);
      $this->pager_element = $query->pager['element'];
    } elseif (is_int($page)) {
      $query->range(0, 1 + $this->options['number_of_items']); //add one so more link knows there's more.
    }
    
    /////sort - if $this doesn't have it, one of the plugins will.
    $sort = $this->options['sort_by'];
    if (method_exists($this, $sort)) {
      $this->$sort($query);
    } else {
      $this->_plugins_invoke($sort, $query);
    }
    $this->_plugins_invoke('sort_alter', $query);
    
    //person, !image
    //event exception.  dletas.

    
    //execute
    $result = $query->execute();
    return (isset($result[$this->entity_type])) ? array_keys($result[$this->entity_type]) : array();
  }
  
  public function query() {
    $efq = new EntityFieldQuery();
    $efq->entityCondition('entity_type', $this->entity_type);
    
    if (module_exists('vsite') && $space = spaces_get_space()) {
      $efq->fieldCondition(OG_AUDIENCE_FIELD, 'target_id', $space->id);
    }
    
    return $efq;
  }
  
  
  /**
   * @function _set_plugins
   * 
   * Instantiates all the plugins this box will use.  Call this on demand instead of on __construct,
   * because the plugins loaded will depend on context.  A node box has a fixed entity_type.  During
   * render, its bundle is fixed, but in the options form, its bundle can change.
   * 
   * Re-run with new criteria to reset.  This way if a box's option form loads all options for its
   * entity_type, the render method can reset to only those applicable to the bundle.   
   */
  private function _set_plugins($entity_type = NULL, $bundle = NULL) {
    $this->plugins = array();
    
    foreach (os_sv_list_plugins($entity_type, $bundle) as $plugin) {
      $class = ctools_plugin_get_class($plugin, 'handler');
      $plugin['instance'] = new $class;
      $this->plugins[$plugin['name']] = $plugin;
    }
    
    return $this->plugins;
  }
  
  /**
   * @function _plugins_invoke
   *
   * Runs a method on all loaded plugins.  Plugin return values will return in
   * keyed array.
   */
  private function _plugins_invoke($method, &$args) {
    $options = $this->options + $this->options_defaults();
    foreach ($this->plugins as $name => $plugin) {
      if (method_exists($plugin['instance'], $method)) {
        $ret[$plugin['name']] = $plugin['instance']->$method($options, $args); //to pass by ref we can't use func_get_args
      }
    }
    
    return (isset($ret)) ? $ret : array();
  }
  
  /**
   * @function entity_view_modes
   * 
   * Wrapper for os_entity_view_modes.  Also derives common view modes
   * available to all content types.
   */
  private function entity_view_modes() {
    $modes = os_entity_view_modes($this->entity_type);
    $all = array_reduce($modes, 'array_intersect', current($modes)); //starting with an empty array means we'll get an empty back.
    $modes['all'] = array_values($all);  
    return $modes;
  }
    
  protected function _display_options() {
    //list all the available view modes for this entity
    $view_modes = os_entity_view_modes($this->entity_type);
    $view_modes = array_unique(array_reduce($view_modes, 'array_merge', array())); 
    
    //get labels for these view modes
    $info = entity_get_info($this->entity_type);
    $options = array_reduce($view_modes, function($ret, $mode) use ($info) {$ret[$mode] = $info['view modes'][$mode]['label']; return $ret;});
    return $options;    
    //get labels
    
    //deprecate the following options...
//     return array(
//       'display_title' => t('Title'),
//       'display_sidebar_teaser' => t('Sidebar Teaser'),
//       'display_teaser' => t('Teaser'),
//       'display_body' => t('Entire Post'),
//       'display_links' => t('Link'),
//     );
  }
  
  protected function _sort_options() {
    return array(
      // Content type sensitive sorts.  // in the future get these from sv render plugins
      'sort_event' => t('Event Date'),
      'sort_presentation' => t('Recently Presented'),
      'sort_biblio' => t('Year of Publication'),
      'sort_news' => t('News Date'),
    
      // Standard sorts,
      'sort_newest' => t('Newest Post'),
      'sort_oldest' => t('Oldest Post'),
      'sort_alpha' => t('Alphabetical'),
      'sort_random' => t('Random'),
    );
  }

  function sort_random(&$efq) {
    $efq->addTag('random');
  }
  
}
