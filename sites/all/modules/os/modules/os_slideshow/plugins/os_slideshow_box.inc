<?php

/**
 * @file
 * The class for "Slideshow" type widgets.
 */

class os_boxes_slideshow extends os_boxes_default {

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();

    $options['gallery'] = 'all';
    $options['size'] = '480x270';
    $options['effect'] = 'scrollLeft';
    $options['style'] = '';
    $options['number_of_items'] = 10;

    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){
    // gallery
    $options = array(
      'all' => t('All')
    );
    // for each gallery node, add nid/node_title to the select options
    $view = views_get_view("scholar_image_gallery");
    $view->set_display('page_1'); //block 1 showed all content
    $view->execute();
    foreach ( $view->result as $r ) {
      $options[$r->nid] = $r->node_title;
    }

    $form['gallery'] = array(
      '#type' => 'select',
      '#title' => t('Gallery'),
      '#options' => $options,
      '#default_value' => $this->options['gallery'],
      '#required' => TRUE
    );

    // effect
    module_load_all_includes('views_slideshow.inc');
    $view = views_get_view('os_slideshow');

    if($view){
	    $view->set_display('default');
	    $view->init_style();

	    $function = 'views_slideshow_singleframe_views_slideshow_options_form';
	    $form_element = array();
	    $form_element_state = array();
	    $views_style_plugin = $view->style_plugin;

	    $function($form_element, $form_element_state, $views_style_plugin);
	    //dpm($form_element);


	    $form['effect'] = $form_element['views_slideshow_singleframe']['effect'];
	    $form['effect']['#default_value'] = $this->options['effect'];
    }

    // size
    $form['size'] = array(
      '#type' => 'textfield',
      '#title' => t('Size'),
      '#default_value' => $this->options['size'],
      '#field_suffix' => 'px',
      '#description' => t('Enter the width of the image in pixels. It should be between 400 and 1000 pixels.'),
      '#process' => array('os_slideshow_box_attach_range'),
      '#element_validate' => array('os_slideshow_box_accepted_range'),
    );

    //dpm($imagecache_presets);
    // nr of items
    $form['number_of_items'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of items to display'),
      '#default_value' => $this->options['number_of_items']
    );

    $form['style'] = array(
      '#title' => t('Player Styles'),
      '#type' => 'radios',
      '#options' => array('' => 'Dark', '_light' => 'Light'),
      '#default_value' => $this->options['style'],
      '#pre_render' => array('os_slideshow_box_style_thumbs'),
      '#attributes' => array('class' => 'thumbnails'),
      '#description' => t('Choose between a black or light grey player style.'),
    );

    $form += parent::options_form();
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {

    drupal_add_css(drupal_get_path('module', 'os_slideshow').'/theme/os-slideshow'.$this->options['style'].'.css');

    $view = views_get_view('os_slideshow');
    $view->set_display('default');
    $view->display_handler->set_option('items_per_page', (int)$this->options['number_of_items']);


    $handler = $view -> display_handler;

    // override effect
    $style_opts = $handler -> options['style_options'];
    $style_opts['views_slideshow_singleframe']['effect'] = $this->options['effect'];
    $handler -> override_option('style_options', $style_opts);

    $imagecache_presets = imagecache_presets();
    $tmpstr = 'os_slideshow_';
    foreach ( $imagecache_presets as $name => $presets ) {
      if ($tmpstr == substr($name, 0, strlen($tmpstr))) {
        $dim = substr($name, strlen($tmpstr));
        $width = explode('x',$dim);
        $options[$width[0]] = $dim;
      }
    }

    // loops through all the image cache presets and finds the next largest one
    // from the user-provided size.
    // Insantly breaking will result in $width referring to the width we want
    // if the user-given size is larger than all of them, we default to the largest available
    foreach ($options as $width => $dim) {
      if ($this->options['size'] <= $width) {
        $ratio = explode('x',$dim);
        $ratio = $ratio[1]/$ratio[0];
        break;
      }
    }

    // override size (imagecache preset)
    $fields_opts = $handler -> options['fields'];
    $fields_opts['field_gallery_image_fid']['format'] = $tmpstr.$dim . '_default';
    $handler -> override_option('fields', $fields_opts);

    if ($this->options['gallery'] != 'all') {
      $view->set_arguments(array($this->options['gallery']));
    }

    $block = parent::render();
    $output = $view->execute_display();

    // swap out the dimenions of the preset with the dimenions the user wants
    $patt_w = '/(<img [^>]* width=")(\d*)("[^>]*>)/';
    $patt_h = '/(<img [^>]* height=")(\d*)("[^>]*>)/';

    $output = preg_replace($patt_w, '${1}'.$this->options['size'].'${3}', $output);
    $output = preg_replace($patt_h, '${1}'.($this->options['size']*$ratio).'${3}', $output);

    $id = str_replace('_', '-', $tmpstr.$dim);
    $block['content'] = '<div id="' . $id. '" style="width: '.($this->options['size']+8).'px">' .$output . "</div>";
    return $block;
  }
}

/**
 * Add the js that will attach a slider to form elements
 * @param unknown_type $element
 */
function os_slideshow_box_attach_range($element) {
  drupal_add_js(drupal_get_path('module', 'os_slideshow').'/theme/os_slideshow_form.js');
  return $element;
}

/**
 * Ensures the value given is within acceptable ranges
 */
function os_slideshow_box_accepted_range($element, &$form_state) {
  if ($element['#value'] < 400 || $element['#value'] > 1000) {
    form_error($element, t('Slideshow width out of bounds: Width should be between 400 and 1000 pixels.'));
  }
}