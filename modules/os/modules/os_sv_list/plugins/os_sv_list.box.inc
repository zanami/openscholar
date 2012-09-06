<?php
class os_sv_list_box extends os_boxes_default {
  
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    
    $options += array(
      'content_type' => 'all',
      'display' => 'display_title',
      'more' => FALSE,
      'more_text' => t('More'),
      'more_link' => 'node', // what's a good default?
      
      'pager' => FALSE,
      'number_of_items' => 5,
      'sort_by' => 'sort_newest'
    );
    
    return $options;
  }
  
  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);
    
    $content_types = array(
      'all' => t('All')
    ) + os_get_bundles(1);
    
    $form['#submit'] = array(
      'os_sv_list_form_submit'
    );
    
    $form['content_type'] = array(
      '#type' => 'select',
      '#title' => 'Content Type',
      '#default_value' => $this->options['content_type'],
      '#options' => $content_types
    );
    
    $form['display'] = array(
      '#type' => 'select',
      '#title' => 'Display style',
      '#default_value' => $this->options['display'],
      '#options' => array(
        'display_title' => t('Title'),
        'display_teaser' => t('Teaser'),
        'display_body' => t('Entire Post')
      )
    );
    
    $form['sort_by'] = array(
      '#type' => 'select',
      '#title' => 'Sorting',
      '#default_value' => $this->options['sort_by'],
      '#options' => array(
        'sort_newest' => t('Newest'),
        'sort_oldest' => t('Oldest'),
        'sort_alpha' => t('Alphabetical')
      )
    );
    
    $form['more'] = array(
      '#title' => 'Show "more" link',
      '#type' => 'checkbox',
      '#default_value' => $this->options['more']
    );
    
    $form['more_link'] = array(
      '#title' => 'URL for the more link',
      '#type' => 'textfield',
      '#default_value' => $this->options['more_link'],
      '#states' => array(
        'visible' => array('input[name=more]' => array('checked' => TRUE))
      )
    );
    
    $form['more_text'] = array(
      '#title' => 'Label of the more link (default: "more")',
      '#type' => 'textfield',
      '#default_value' => $this->options['more_text'],
      '#states' => array(
        'visible' => array('input[name=more]' => array('checked' => TRUE))
      )
    );
    
    $form['pager'] = array(
      '#title' => 'Show pager',
      '#type' => 'checkbox',
      '#default_value' => $this->options['pager']
    );
    
    $form['number_of_items'] = array(
      '#title' => 'Number of items to display',
      '#type' => 'select',
      '#options' => array_slice(range(0, 20), 1, NULL, TRUE),
      '#default_value' => $this->options['number_of_items']
    );
    
    $form = $this->form_content_types($form, $content_types);
    
    return $form;
  }
  /**
   * // * Implementation of boxes_content::render().
   */
  public function render() {
    $block = parent::render();
    
    $efq = $this->init_query();
    
    // content type
    if ($this->options['content_type'] != 'all') {
      $efq->propertyCondition('type', $this->options['content_type']);
    }
    
    // item limit
    $efq->pager($this->options['number_of_items']);
    
    // sort
    $sort_func = (method_exists($this, $this->options['sort_by'])) ? $this->options['sort_by'] : 'sort_newest';
    $this->$sort_func($efq);
    
    // display
    $result = $efq->execute();
    if (isset($result['node']) && count($result['node'])) {
      $display_func = (method_exists($this, $this->options['display'])) ? $this->options['display'] : 'display_title';
      $nodes = node_load_multiple(array_keys($result['node']));
      $block['content'] = $this->$display_func($nodes);
    }
    
    // pager
    if ($this->options['pager']) {
      // theme_pager();
      $parameters = array(
        'sv_list_box_delta' => $this->delta
      );
      $pager = theme('pager', array(
        'parameters' => $parameters
      ));
      
      //add use-ajax class and replace the link.  theme_pager won't pass these along all the way to the links
      $pager = preg_replace('/<a /', '<a class="use-ajax" ', $pager);
      $pager = preg_replace('/href="(.*?)\?/', 'href="os_sv_list_ajax/nojs?dest=$1&amp;', $pager);
      
      $block['content'] .= $pager;
    }
    
    // more link
    if ($this->options['more']) {
      $block['content'] .= l($this->options['more_text'], $this->options['more_link']);
    }
    
    drupal_add_library('system', 'drupal.ajax');
    $block['content'] = '<div id="box-' . $this->delta . '-wrapper">' . $block['content'] . '</div>';
    
    return $block;
  }
  
  // init_query - humble beginnings of the EFQ
  function init_query() {
    // defaults
    $efq = new EntityFieldQuery();
    $efq->entityCondition('entity_type', 'node');
    $efq->propertyCondition('status', 1);
    $efq->propertyCondition('type', array_keys(os_get_bundles(1)), 'IN');
    
    // vsite
    if (module_exists('vsite') && $space = spaces_get_space()) {
      $efq->fieldCondition('field_og_general', 'target_id', $space->id);
    }
    
    return $efq;
  }
  
  /**
   * @function form_content_types
   *
   * Checks if any content types change the form options. Form alters are
   * provided by $this->content_alter_form_CONTENTTYPE()
   * See os_sv_list_form_boxes_box_form_alter and
   * os_sv_list_boxes_box_form_submit for how the content form options get
   * nudged into
   * the right place.
   *
   * Note that adding an element to a form probably won't work. You can add
   * options to a select and provide a function, but putting
   * something totally new in won't work since render() won't have any way to
   * handle the new element and the box won't store the option
   * anyway.
   */
  function form_content_types($form, $content_types) {
    // make some weights. content specific elements should appear in same
    // position as original element.
    $weight = 0;
    foreach ( $form as $key => $value ) {
      if (empty($form[$key]['#weight'])) {
        $form[$key]['#weight'] = $weight;
        $weight += 100;
      }
    }
    
    $original_form = $form; // array clone. we're modifying as we go but need to
                            // be able to compare to the original
    foreach ( array_keys($content_types) as $ct ) {
      $method = 'content_alter_form_' . $ct;
      if (method_exists($this, $method)) {
        $altered = $this->$method($original_form);
        
        // find what changed. add form #state and append to both forms
        foreach ( array_keys($original_form) as $elem ) {
          if ($original_form[$elem] != $altered[$elem]) {
            $altered[$elem]['#states'] = array(
              'visible' => array('select[name=content_type]' => array('value' => $ct))
            );
            $form["${ct}_${elem}"] = $altered[$elem];
            
            $form[$elem]['#states']['invisible']['select[name=content_type]'][] = array(
              'value' => $ct
            );
          }
        }
      }
    }
    
    return $form;
  }
  
  // events sort by date ascending, starting now, not in past.
  function content_alter_form_event($form) {
    $form['sort_by']['#options'] = array('sort_event' => t('Event Date')) + $form['sort_by']['#options'];
    
    if ($this->new) {
      $form['sort_by']['#default_value'] = 'sort_event';
    }
    
    return $form;
  }
  
  /**
   * sorting functions *
   */
  function sort_newest(&$efq) {
    $efq->propertyOrderBy('created', 'DESC');
  }
  function sort_oldest(&$efq) {
    $efq->propertyOrderBy('created', 'ASC');
  }
  function sort_alpha(&$efq) {
    $efq->propertyOrderBy('title', 'ASC');
  }
  
  function sort_event(&$efq) {
    $efq->fieldOrderBy('field_date', 'value', 'ASC');
    $efq->fieldCondition('field_date', 'value', date('Y-m-d'), '>=');
  }
  
  /**
   * display functions *
   */
  function display_body($nodes) {
    $items = node_view_multiple($nodes, 'body');
    return drupal_render($items);
  }
  function display_teaser($nodes) {
    $items = node_view_multiple($nodes, 'teaser');
    return drupal_render($items);
  }
  function display_title($nodes) {
    foreach ( $nodes as $nid => $node ) {
      $items[] = l($node->title, 'node/' . $nid);
    }
    
    return theme('item_list', array(
      'items' => $items,
      'type' => 'ul'
    ));
  }
}