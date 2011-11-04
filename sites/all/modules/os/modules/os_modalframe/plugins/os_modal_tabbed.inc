<?php

class os_modal_tabbed extends os_boxes_modalframe {

  protected function config_path() {
    return 'cp/os-modalframe/'.($this->delta?$this->delta:('boxes_add__'.$this->plugin_key));
  }

  public function form_alter(&$form, &$form_state) {
    parent::form_alter($form, $form_state);
    unset($form['submit']['#attributes']);
  }

  public static function submit(&$form, &$form_state) {
    return self::$latest;
  }

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_form();
    $options['tabs'] = (object)array(
      'default_tab' => FALSE,
      'tabs' => array(),
    );
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form() {
    $form = parent::options_form();

    ctools_include('modal');
    ctools_include('ajax');

    $widgets = vsite_layout_get_widgets();
    //vsite_layout_ui_check_for_disabled_widgets($widgets);

    // filter out factories
    $options = array('' => 'Select Widget');
    foreach ($widgets as $wgt => $info) {
      if ($info['factory']) {
        unset($widgets[$wgt]);
      }
      else {
        $options['boxes-'.$wgt] = $info['label'];
      }
    }

    $qt = $this->options['tabs'];
    $new_tab = array(
      'type' => 'block',
      'title' => 'New Tab',
      'weight' => 2000000,  // so its always last
      'delta' => '',
      'hide_title' => 0,
    );
    $tabs_data = $qt->tabs + array($new_tab);

    $links = array();
    $fieldset = array();
    $form['tabs'] = array(
      '#prefix' => '<div id="tabs">',
      '#suffix' => '</div>',
    );
    foreach ($tabs_data as $k => $t) {
      $id = 'tab-'.$k;
      $links[$t['weight']] = array(
        'href' => '',
        'fragment' => $id,
        'external' => TRUE,
        'title' => $t['title'],
      );

      $form['tabs'][$id] = array(
        '#type' => 'fieldset',
        '#attributes' => array('id' => $id, 'class' => 'panel'),
        '#tree' => TRUE,
        '#weight' => 1,
        'title' => array(
          '#type' => 'textfield',
          '#default_value' => $t['title'],
        ),
        'delta' => array(
          '#type' => 'select',
          '#title' => t('Widget to Display in this Tab'),
          '#options' => $options,
          '#default_value' => $t['delta'],
        ),
        'hide_title' => array(
          '#type' => 'checkbox',
          '#default_value' => $t['hide_title'],
          '#title' => t('Hide Widget Title'),
        ),
        'weight' => array(
          '#type' => 'hidden',
          '#default_value' => $t['weight'],
        ),
        'type' => array(
          '#type' => 'value',
          '#value' => 'block',
        ),
        'make_default' => array(
          '#type' => 'markup', // drupal doesnt let us make a button that does nothing...
          '#value' => '<input class="make-default" type="button" value="Make Default">',
        ),
      );
    }

    $form['tabs']['links'] = array(
      '#pre_render' => array('os_modal_tabbed_links_prerender'),
      '#value' => $links,
      '#weight' => 0,
    );

    $form['default'] = array(
      '#default_value' => $qt->default_tab,
      '#type' => 'hidden',
    );

    return $form;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function render() {
  }
}

function os_modal_tabbed_links_prerender($elements) {
  $elements['#value'] = theme('links', $elements['#value']);
  return $elements;
}