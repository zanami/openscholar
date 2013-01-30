<?php

class os_slideshow_box extends os_boxes_default {

  public function options_defaults() {
    $options = parent::options_defaults();
    
    return $options + array(
      'pager' => 'bottom',
      'controls' => 'bottom',
      'slide_counter' => 'bottom',
      'effect' => 'fade',
      'size' => 600,
      'nodes' => array(),
        
      //not configurable.  
      'min_width' => 400,
      'max_width' => 980,
    );
  }

  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);
    $defaults = $this->options_defaults();
    
    $form['display'] = array(
      '#type' => 'fieldset',
      '#title' => t('Display Options'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 10,
    );
     
    $form['display']['size'] = array(
      '#type' => 'textfield',
      '#title' => t('Size'),
      '#default_value' => $this->options['size'],
      '#field_suffix' => 'px',
      '#description' => t('Enter the width of the slideshow in pixels. It should be between %min and %max pixels.', array('%min'=>$defaults['min_width'], '%max'=>$defaults['max_width'])),
      '#element_validate' => array('os_slideshow_accepted_range'),
      '#attached' => array(
        'js' => array(
          drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_slideshow/os_slideshow_form.js'
        )
      ),
    );
    
    $display_options = array('pager' => t('Show Pager'), 'controls' => t('Show Controls'), 'slide_counter' => t('Slide Counter'));
    foreach ($display_options as $option => $title)
      $form['display'][$option] = array(
        '#type' => 'hidden',
        '#title' => $title,
        //omitting top/bottom options, but leaving this here so we can add it back in when we're ready for new features.
        //'#options' => array('none' => t('No Display'), 'top' => t('Top'), 'bottom' => t('Bottom')),
        '#options' => array('none' => t('No'), 'bottom' => t('Yes')),
        '#default_value' => $this->options[$option],
      );
   
    //get jquery effects from view (include file hsould arleady be loaded)
    $slideshow_form_full = $state = array();
    if (module_exists('views_slideshow_cycle')) {
      if (!function_exists('views_slideshow_cycle_views_slideshow_slideshow_type_form')) {
        require_once drupal_get_path('module', 'views_slideshow_cycle') . '/views_slideshow_cycle.views_slideshow.inc';
      }
      @views_slideshow_cycle_views_slideshow_slideshow_type_form($slideshow_form_full, $state, $view);
      $form['display']['effect'] = $slideshow_form_full['views_slideshow_cycle']['effect'];
      $form['display']['effect']['#default_value'] = $this->options['effect'];
    }

    $form['#attached']['js'] = array(
      drupal_get_path('module', 'os_slideshow').'/plugins/os_slideshow_box.js',
    );
    
    if (empty($this->options['nodes'])) {
      $form['nodes'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="messages warning">You haven\'t added any slides yet.  Put this widget in your layout and visit that page to add slides.</div>',
        '#weight' => 5,  
      );
    } else {
      // build a normal form
      $form['nodes'] = array(
        '#type' => 'markup',
        '#tree' => TRUE,
        '#pre_render' => array('os_slideshow_draggable_table'),      
        '#weight' => 5,
      );
      
      $nids = $this->_get_nids(); //filter out dead nids
      
      $form['nodes']['#process'][] = 'os_slideshow_process_form_nodes';
      $form['nodes_list'] = array(
        '#type' => 'hidden',
        '#value' => $nids,    
      );
      
      //load slideshow_slide nodes
      uasort($this->options['nodes'], 'element_sort');
      $nodes = node_load_multiple($nids);
  
      foreach ($this->options['nodes'] as $nid => $data) {
        if (!in_array($nid, $nids)) {
          continue;
        }
        
        $uri = $nodes[$nid]->field_image[$nodes[$nid]->language][0]['uri'];
        $img = theme('image_style', array('style_name' => 'os_square_50_50','path' => $uri, 'width'=>50, 'height'=>50, 'attributes' => array('style' => 'float: left; margin-right: 1em;')));
        $title = '<strong>' . $nodes[$nid]->title . '</strong>';
        
        //edit link loads node edit with dest of this page.  this page's dest needs to be preserved.  nested destinations.
        $dest = (isset($_GET['destination'])) ? current_path() . '?destination=' . $_GET['destination'] : current_path();
        $edit_link = l('Edit Slide', "node/${nid}/edit", array('query' => array('destination' => $dest)));
        $remove_link =  '<a href="#" class="remove" title="Remove Slide">Remove</a>';
        $links = "<div>${title}<br />${edit_link} | ${remove_link}</div>";
        
        $form['nodes'][$nid] = array(
          'nid' => array(
            '#type' => 'hidden',
            '#default_value' => $nid,
          ),
          'text' => array(
            //'#markup' => $img . ' <span>' . $title . '</span><br /> ' . $edit_link .  ' | <a href="#" class="remove" title="Remove Slide">Remove</a>',
            '#markup' => $img . $links,
          ),
          '#weight' => $data['weight'],
          'weight' => array(
            '#type' => 'textfield',
            '#default_value' => $data['weight'],
            '#size' => 3,
            '#attributes' => array('class' => array('field-weight')),
          )   
        );
      }
    }
    
    return $form;
  }

  public function render() {
    $block = parent::render();

    //show Add Slide link to admins
    if (spaces_access_admin()) {
      $block['content'] = l('Add Slide', 'node/add/slideshow-slide', array(
        'attributes' => array('class' => array('add_new')),
        'query' => array('widget' => $this->delta, 'destination' => current_path()),
      ));
    }
        
    if ($view = views_get_view('os_slideshow')) {
      $view->init_display();
      $handler = $view->display_handler;
      $style_options = $handler->get_option('style_options');
      
      //add slideshow widgets
      foreach(array('pager', 'controls', 'slide_counter') as $option) {
        foreach($style_options['widgets'] as $position => $info) {
          $style_options['widgets'][$position]['views_slideshow_' . $option]['enable'] = ($position == $this->options[$option]);
        }
      }
      
      //effects
      $style_options['views_slideshow_cycle']['effect'] = $this->options['effect'];
      $handler->set_option('style_options', $style_options);

      //use an appropriately scaled image style.
      $image_style = $this->_get_image_style($this->options['size']);
      $style = current(array_keys($image_style));
      $fields = $handler->get_option('fields');
      $fields['field_image']['settings']['image_style'] = $style;
      $handler->set_option('fields', $fields);
      
      //filter slides  - nid is an ORed group of nids.  all it needs is a filter array per nid.
      if ($this->options['nodes']) {
        $filters = $handler->get_option('filters');
        $nid_filter = $filters['nid'];
        unset($filters['nid']);
        $c = 0;
        foreach (array_keys($this->options['nodes']) as $nid) {
 //       for($c = 0; $c < count($this->options['nodes']); $c++) {
          $nid_filter['value']['value'] = $nid;
          $filters['nid_' . $c++] = $nid_filter;
        }
        $handler->set_option('filters', $filters);
      }
      
      //stash the box delta for hook_pre_render (is there a better way to do this part?)
      $view->set_arguments(array('delta' => $this->delta));
      
      //render output
      $output = $view->preview();     
      $output = $this->_resize_images($output, $image_style);    //use user provided image size
      $output = $this->_remove_dead_links($output);              //all images are linked, even if no link was provided.
      //if we're resizing images in this way, mabye links to the empty string target could be removed the same way.
      
      
      $block['content'] .= $output; 
    }
    return $block;
  }
  

  //returns [name => [width, height]] of style for this box's size
  private function _get_image_style() {
    $image_styles = image_styles();
    $slideshows = preg_grep('/^slideshow_/', array_keys($image_styles));
    $size = (int) $this->options['size'];
    
    $sizes = array();
    foreach ($slideshows as $name) {
      $style = current($image_styles[$name]['effects']);
      $sizes[$style['data']['width']] = array($name => $style['data']);
    }
    
    ksort($sizes);
    foreach ($sizes as $width => $data) {
      if ($width >= (int)$this->options['size']) {
        break;
      }
    }
    
    return $data;    
  }
  
  //resize images provided by the view to the size specified by the user
  private function _resize_images($html, $style) {
    $dim = current($style);
    $preset = current(array_keys($style));
    $ratio = $dim['height']/$dim['width'];
    $width = $this->options['size'];
    $height = $width * $ratio;
    
    // swap out the dimenions of the preset with the dimenions the user wants
    $patt_w = '/(<img [^>]* width=")([^"]*)("[^>]*>)/';
    $patt_h = '/(<img [^>]* height=")([^"]*)("[^>]*>)/';
    $patt_url = '/<img [^>]*src="([^"]*)"[^>]*>/';
    
    $html = preg_replace($patt_w, '${1}'.($width).'px'.'${3}', $html);
    $html = preg_replace($patt_h, '${1}'.($height).'px'.'${3}', $html);
//     $matches = array();
//     preg_match_all($patt_url, $html, $matches, PREG_SET_ORDER);
//     foreach ($matches as $m) {
       //$html = str_replace($m[1], image_style_url($preset, $m[1]), $html);  
//     }
    
    return $html;
  }
  
  private function _remove_dead_links($html) {
    $html;
    //$link = '<a href="' . current_path() . '%20">';
    return $html; 
  }
  
  //filters out dead nids
  private function _get_nids() {
    $nids = array_keys($this->options['nodes']);
    
    if ($nids) {
      $nids = db_select('node', 'n')
        ->fields('n', array('nid'))
        ->condition('n.nid', $nids, 'IN')
        ->condition('n.type', 'slideshow_slide')
        ->execute()
        ->fetchCol();
    }
    
    return $nids;
  }
}


function os_slideshow_draggable_table($element) {
  // render the form as a table
  $rows = array();
  uasort($element, 'element_sort');
  foreach (element_children($element) as $c) {
    $rows[] = array(
      'data' => array(
        drupal_render($element[$c]['nid']) . drupal_render($element[$c]['text']),
        drupal_render($element[$c]['weight']),
      ),
      'class' => ((string)$c!='blank') ? array('draggable') : array(),
    );
  }
  $element['#headers'] = array();
  $element['#attributes'] = array('id' => 'slideshow-slides-list');
  $element['#rows'] = $rows;
  $element['#sticky'] = false;
  $element['#empty'] = true;
  //$element['#value'] = theme('table', array(), $rows, );
  $element['#theme'] = 'table';
  drupal_add_tabledrag('slideshow-slides-list', 'order', 'sibling', 'field-weight');
  return $element;
}

/**
 * Ensures the value given is within acceptable ranges
 */
function os_slideshow_accepted_range($element, &$form_state) {
  $defaults = $form_state['box']->options_defaults();
  if ($element['#value'] < $defaults['min_width'] || $element['#value'] > $defaults['max_width']) {
    form_error($element, t('Slideshow width out of bounds: Width should be between %min and %max pixels.', array('%min'=>$defaults['min_width'], '%max'=>$defaults['max_width'])));
  }
  $element['#value'] = (int)$element['#value'];
}

