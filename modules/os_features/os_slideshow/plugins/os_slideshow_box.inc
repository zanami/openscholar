<?php

class os_slideshow_box extends os_boxes_default {

  public function options_defaults() {
    $options = parent::options_defaults();
    
    return $options + array(
      'pager' => 'none',
      'controls' => 'none',
      'slide_counter' => 'none',
      'show_image_titles' => 'none',
      'effect' => 'fade',
      'style' => 'dark',
      'size' => 600,
      'nodes' => array(),
    );
  }

  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);
    
    $form['display'] = array(
      '#type' => 'fieldset',
      '#title' => t('Display Options'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    
    $form['display']['style'] = array(
      '#title' => t('Player Styles'),
      '#type' => 'select',
      '#options' => array('dark' => t('Dark'), 'light' => t('Light')),
      '#default_value' => $this->options['style'],
      //'#pre_render' => array('os_slideshow_box_style_thumbs'),
      '#attributes' => array('class' => array('thumbnails')),
      '#description' => t('Choose between a black or light grey player style.'),
    );
    
    $form['display']['size'] = array(
      '#type' => 'textfield',
      '#title' => t('Size'),
      '#default_value' => $this->options['size'],
      '#field_suffix' => 'px',
      '#description' => t('Enter the width of the slideshow in pixels. It should be between 400 and 960 pixels.'),
      //'#element_validate' => array('os_boxes_slideshow_accepted_range'),
      '#attached' => array(
        'js' => array(
          drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_slideshow/os_slideshow_form.js'
        )
      ),
    );
    
    $display_options = array('pager' => t('Show Pager'), 'controls' => t('Show Controls'), 'slide_counter' => t('Slide Counter'));
    foreach ($display_options as $option => $title)
      $form['display'][$option] = array(
        '#type' => 'select',
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
    
  //  return $form;
    
    

    $form['#attached']['js'] = array(
      drupal_get_path('module', 'os_slideshow').'/plugins/os_slideshow_box.js',
    );

    // build a normal form
    $form['nodes'] = array(
      '#type' => 'markup',
      '#tree' => TRUE,
      '#pre_render' => array('os_slideshow_draggable_table'),
    );
    
    //load slideshow_image nodes
    uasort($this->options['nodes'], 'element_sort');
    $nodes = node_load_multiple(array_keys($this->options['nodes']));

    foreach ($this->options['nodes'] as $nid => $data) {
      $uri = $nodes[$nid]->field_image[$nodes[$nid]->language][0]['uri'];
      $img = theme('image_style', array('style_name' => 'os_square_50_50','path' => $uri, 'width'=>50, 'height'=>50));
      $title = $nodes[$nid]->title;
      $form['nodes'][$nid] = array(
        'nid' => array(
          '#type' => 'hidden',
          '#default_value' => $nid,
        ),
        'text' => array(
          '#markup' => $img . ' <span>' . $title . '</span> <a href="#" class="remove" title="Remove Slide">Remove</a>',
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

    //not doing new ones in here...
//     $form['count'] = array(
//       '#type' => 'hidden',
//       '#default_value' => count($this->options['nodes'])-1,
//       '#attributes' => array(
//         'id' => 'edit-count',
//       ),
//       '#process' => array('os_boxes_manual_list_add_new_fields'),
//     );
    return $form;
  }

  public function render() {
    dpm($this->options['nodes']);
    $block = parent::render();

    //show Add Slide link to admins
    if (spaces_access_admin()) {
      $block['content'] = l('Add Slide', 'node/add/slideshow-image', array(
        'attributes' => array('class' => array('add_new')),
        'query' => array('widget' => $this->delta, 'destination' => current_path()),
      ));
    }
    
    drupal_add_css(drupal_get_path('module', 'os_boxes') . '/plugins/os_boxes_slideshow/theme/os-slideshow-' . $this->options['style'] . '.css');
    drupal_add_css(drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_slideshow/os_boxes_slideshow.css');
    
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

// function os_boxes_manual_list_add_new_fields($element, &$form_state, &$form) {
//   for ($i=$element['#default_value']; $i < $element['#value']; $i++) {
//     $form['options']['nodes'][$i] = array(
//       '#attributes' => array('id' => $i),
//       '#parents' => array('nodes', $i),
//       '#array_parents' => array('options', 'nodes', $i),
//       '#tree' => TRUE,
//     );
//     $form['options']['nodes'][$i] += $form['options']['nodes']['blank'];
//     foreach ($form['options']['nodes'][$i] as $key => &$child) {
//       if (strpos($key, '#') !== 0) {
//         unset($child['#parents']);
//         unset($child['#name']);
//         unset($child['#value']);
//       }
//     }
//     form_builder($form['form_id']['#value'], $form['options']['nodes'][$i], $form_state);
//   }
//   unset($form_state['values']['nodes']['blank']);

//   // remove nids that aren't in the post
//   if (is_array($form_state['values'])) {
//     foreach ($form_state['values']['nodes'] as $k => $v) {
//       if (!isset($form_state['input']['nodes'][$k]) || !isset($form_state['input']['nodes'][$k]['nid'])) {
//         unset($form_state['values']['nodes'][$k]);
//       }
//     }
//   }

//   return $element;
// }
