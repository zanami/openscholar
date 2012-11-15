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
      'more_link' => '', // what's a good default?

      'pager' => FALSE,
      'number_of_items' => 6,
      'sort_by' => 'sort_newest',

      'biblio_show_all' => TRUE,
      'biblio_whitelist' => array('all'=>'all'),
    );

    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);
    $defaults = $this->options_defaults();
    drupal_add_js(array('os_sv_list_box' => array('new_box' => $this->new)), 'setting');

    $content_types = array(
      'all' => t('All')
    ) + os_get_bundles(1);

    if (isset($content_types['media_gallery'])) {
      $content_types['image'] = t('Images');
    }

    $form['#submit'] = array(
      'os_sv_list_form_submit'
    );

    $form['content_type'] = array(
      '#type' => 'select',
      '#title' => 'Content Type',
      '#default_value' => $this->options['content_type'],
      '#options' => $content_types,
      '#id' => 'os_sv_list_content_type',
      '#ajax' => array(
        'callback' => 'os_sv_list_more_link_ajax',
        //'wrapper' => 'more_link_div',
        'wrapper' => 'edit-more-link',
        'method' => 'replace',
        'effect' => 'none',
      ),
      '#attached' => array(
        'js' => array(
          drupal_get_path('module', 'os_sv_list').'/os_sv_list.js',
        ),
      ),
    );

    $form['biblio_show_all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show all publication types'),
      '#default_value' => isset($this->options['biblio_show_all']) ? $this->options['biblio_show_all'] : $defaults['biblio_show_all'],
      '#id' => 'biblio_show_all_check',
      '#prefix' => '<div id="os_sv_list_biblio_show_all">',
      '#suffix' => '</div>',
    );

    $form['biblio_whitelist'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Select publication types to display'),
      '#default_value' => isset($this->options['biblio_whitelist']) ? $this->options['biblio_whitelist'] : $defaults['biblio_whitelist'],
      '#options' => $this->_biblio_types(),
      '#prefix' => '<div id="os_sv_list_biblio_whitelist">',
      '#suffix' => '</div>',
      '#attached' => array(
        'js' => array(
          drupal_get_path('module', 'os_boxes').'/misc/selectAll.js',
        ),
      ),
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
        //content type sensitive sorts
        'sort_event' => t('Event Date'),
        'sort_presentation' => t('Recently Presented'),
        'sort_biblio' => t('Year of Publication'),

        //standard sorts
        'sort_newest' => t('Newest Post'),
        'sort_oldest' => t('Oldest Post'),
        'sort_alpha' => t('Alphabetical'),
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
        'visible' => array('input[name=more]' => array('checked' => TRUE)),
        'invisible'  => array('input[name=more]' => array('checked' => FALSE))

      ),
      '#prefix' => '<div id="more_link_div">',
      '#suffix' => '</div>',
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

//    $form = $this->form_content_types($form, $content_types);

    return $form;
  }
  /**
   * // * Implementation of boxes_content::render().
   */
  public function render() {
    $block = parent::render();
    $options = $this->options;
    $page_num = isset($_GET['page']) ? $_GET['page'] : 0;
    $page = 0;

    $nids = $this->get_nids($page_num, $page);

    $more_link = FALSE;

    if (count($nids)) {
      if (count($nids) >= $options['number_of_items']) {
        $nids = array_slice($nids, 0, $options['number_of_items']);
        $more_link = TRUE;
      }

      $display_func = (method_exists($this, $options['display'])) ? $options['display'] : 'display_title';
      $nodes = node_load_multiple($nids);

      if ($options['content_type'] == 'image') {
        $display_func = 'display_image';
        $nodes = file_load_multiple($nids);
      }

      $block['content'] = $this->$display_func($nodes);
    }

    // pager widget
    $pager = '';
    if ($options['pager'] && $block['content']) {
      // theme_pager();
      $parameters = array(
        'sv_list_box_delta' => $this->delta,
        'pager_id' => $page,
      );
      $pager = theme('views_mini_pager', array(
        'parameters' => $parameters,
        'element' => $page,
        'quantity' => $options['number_of_items'],
      ));
    }

    // more link
    if ($options['more'] && $more_link && $block['content']) {
      $block['content'] .= l($options['more_text'], $options['more_link'], array('attributes' => array('class' => array('more-link'))));
    }

    drupal_add_library('system', 'drupal.ajax');
    drupal_add_js(drupal_get_path('module', 'os_sv_list'). '/os_sv_list_display.js');

    if ($block['content']) {
      $block['content'] = '<div id="box-' . $this->delta . '-page" data-page="'.$page_num.'" data-delta="'.$this->delta.'">' . $block['content'] . '</div>' . $pager;
      return $block;
    } else {
      $block['title'] = $block['subject'] = '';
      return $block;
    }
  }

  // get_default_query_query - humble beginnings of the EFQ
  function get_default_query() {
    // defaults
    $efq = new EntityFieldQuery();
    $efq->entityCondition('entity_type', 'node');
    $efq->propertyCondition('status', 1);
    $efq->propertyCondition('type', array_keys(os_get_bundles(1)), 'IN');

    // vsite
    if (module_exists('vsite') && $space = spaces_get_space()) {
      $efq->fieldCondition(OG_AUDIENCE_FIELD, 'target_id', $space->id);
    }

    return $efq;
  }

  /**
   * Queries the DB for all nids in the box
   *
   * @param int $page_num
   *   Specifies a page.  FALSE to get all nids.
   * @return multitype:
   */
  public function get_nids($page_num = FALSE, &$page = NULL) {
    $efq = $this->get_default_query();
    $options = $this->options;

    //exceptions
    switch ($options['content_type']) {
      case 'book':
        //limit book depth to 1.  not a field, so use a regular query
        $query = db_select('menu_links', 'ml')
        ->condition('module', 'book')
        ->condition('depth', 1);
        $query->addField('ml', 'link_path');
        $book_nids = $query->execute()->fetchCol();
        $book_nids = preg_replace('/node\//', '', $book_nids);
        $book_nids = array_values($book_nids);

        if (count($book_nids)) {
          $efq->propertyCondition('nid', $book_nids, 'IN');
        }
        break;

      case 'biblio':
        $page = 0; //add pager properly later
        $items = $options['number_of_items'];
        $query = db_select('node', 'n')
        ->condition('n.type', 'biblio')
        ->condition('n.status', 1);

        if (module_exists('vsite') && $space = spaces_get_space()) {
          $query->join('og_membership', 'og', 'og.etid = n.nid');
          $query->condition('og.gid', $space->id);
          $query->condition('og.entity_type', 'node');
          $query->condition('og.group_type', 'node');
        }

        $query->join('biblio', 'b', 'n.vid = b.vid');

        if (isset($options['biblio_show_all']) && !$options['biblio_show_all'] && isset($options['biblio_whitelist'])) {
          $bib_types = array_filter($options['biblio_whitelist']);
          $query->condition('biblio_type', array_values($bib_types), 'IN');
        }

        switch($options['sort_by']) {
          case 'sort_biblio': $query->orderBy('biblio_year', 'DESC'); break;
          case 'sort_newest': $query->orderBy('n.created', 'DESC'); break;
          case 'sort_oldest': $query->orderBy('n.created', 'ASC'); break;
          case 'sort_alpha':  $query->orderBy('n.title', 'ASC'); break;
        }

        if ($page_num) {
          $query->range($page_num*$items, $items);
        }
        $query->fields('n', array('nid'));

        $biblio_nids = $query->execute()->fetchCol();
        break;

      case 'image':
        $image_query = new EntityFieldQuery();
        $image_query->entityCondition('entity_type', 'file');
        if ($space = spaces_get_space()) {
          $image_query->fieldCondition(OG_AUDIENCE_FIELD, 'target_id', $space->id);
        }

        $sorts = array(
          'sort_newest' => array('timestamp', 'DESC'),
          'sort_oldest' => array('timestamp', 'ASC'),
          'sort_alpha' => array('filename', 'ASC'),
        );
        $sort = (isset($sorts[$options['sort_by']])) ? $sorts[$options['sort_by']] : $sorts['sort_newest'];
        $image_query->propertyOrderBy($sort[0], $sort[1]);

        if ($options['pager']) {
          $image_query->pager($options['number_of_items']);
          $image_page = $image_query->pager['element'];
        }
        break;
    }

    // content type
    if ($options['content_type'] != 'all') {
      $efq->propertyCondition('type', $options['content_type']);
    }

    // pager for efq
    if ($options['pager'] && $options['content_type'] != 'biblio' && is_int($page)) {
      $efq->pager($options['number_of_items']);
      $page = $efq->pager['element'];
    } elseif ($options['pager'] && $options['content_type'] == 'biblio' && empty($page_num)) {
      //init the pager since we don't have efq doing that for us.
      $total = count($biblio_nids);
      $limit = $options['number_of_items'];
      pager_default_initialize($total, $limit, $page);
    }

    // sort
    $sort_func = (method_exists($this, $options['sort_by'])) ? $options['sort_by'] : 'sort_newest';
    $this->$sort_func($efq);

    // display
    switch ($options['content_type']) {
      case 'biblio':
        $nids = $biblio_nids;
        break;

      case 'image':
        $result = $image_query->execute();
        $nids = isset($result['file']) ? array_keys($result['file']) : array();
        $page = (isset($image_page)) ? $image_page : NULL;
        break;

      default:
        $result = $efq->execute();
        $nids = isset($result['node']) ? array_keys($result['node']) : array();
        break;
    }

    return $nids;
  }


  /**
   * sorting functions
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

  function sort_presentation(&$efq) {
    $efq->fieldOrderBy('field_presentation_date', 'value', 'DESC');
    $this->sort_newest($efq);
  }
  /**
   * display functions
   */
  function display_body($nodes) {
    $items = node_view_multiple($nodes, 'full');
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

  function display_image($files) {
    $html = '';
    foreach ($files as $image) {
      $variables = array(
        'path' => $image->uri,
        'attributes' => array(),
      );
      $html .= theme('image', $variables);
    }
    return $html;
  }

  function _biblio_types() {
    $rows = db_select('biblio_types', 'bt')
      ->fields('bt', array('tid', 'name'))
      ->condition('tid', 0, '>')
      ->condition('visible', 1)
      ->execute();
    $options = array('all' => 'Select All');
    foreach ($rows as $r) {
      $options[$r->tid] = $r->name;
    }

    return $options;
  }

}
