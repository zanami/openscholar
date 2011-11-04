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

    $widgets = vsite_layout_get_widgets();
    //vsite_layout_ui_check_for_disabled_widgets($widgets);

    // filter out factories
    $options = array('os_modal_tabbed-remove' => t('No Widget'));
    foreach ($widgets as $wgt => $info) {
      if ($info['factory']) {
        unset($widgets[$wgt]);
      }
      else {
        $options['boxes-'.$wgt] = $info['label'];
      }
    }

    $qt = $this->options['tabs'];
    $existing = $qt->tabs;
    if (!count($existing)) {
      $existing[] = array(
        'type' => 'block',
        'title' => t('First Tab'),
        'weight' => 0,
        'delta' => 'os_modal_tabbed-remove',
        'hide_title' => 0,
      );
    }
    $new_tab = array(
      'type' => 'block',
      'title' => 'New Tab',
      'weight' => 'new',  // special case
      'delta' => 'os_modal_tabbed-remove',
      'hide_title' => 0,
    );
    $tabs_data = array_merge($existing, array('new' => $new_tab));

    $links = array();

    // stores the number of real tabs
    // as soon as user picks a widget, this is incremented with js
    $form['tab_count'] = array(
      '#type' => 'hidden',
      '#default_value' => count($existing),
      '#process' => array('os_modal_tabbed_add_new_tabs'),
    );

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
          '#title' => t('Title of the Tab'),
          '#type' => 'textfield',
          '#default_value' => $t['title'],
        ),
        'delta' => array(
          '#type' => 'select',
          '#title' => t('Widget to Display in this Tab'),
          '#options' => $options,
          '#default_value' => $t['delta'],
          '#description' => t('Choose which widget to display in this tab. Select \'No Widget\' to remove this tab.')
        ),
        'hide_title' => array(
          '#type' => 'checkbox',
          '#default_value' => $t['hide_title'],
          '#title' => t('Hide Selected Widget\'s Title'),
        ),
        'weight' => array(
          '#type' => 'hidden',
          '#default_value' => is_numeric($t['weight'])?$t['weight']:2000000,
          '#attributes' => array('class' => 'tab-weight'),  // for js
        ),
        'type' => array(
          '#type' => 'value',
          '#value' => 'block',
        ),
        'make_default' => array(
          '#type' => 'markup', // drupal doesnt let us make a button that does nothing...
          '#value' => '<input class="make-default" panel="'.$id.'" type="button" value="Make Default">',
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

    $form['#after_build'][] = 'os_modal_tabbed_add_form_externals';

    return $form;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function render() {
  }
}

/**
 * Includes the necessary js and css
 */
function os_modal_tabbed_add_form_externals($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'os_modalframe').'/theme/os_modal_tabbed.css');
  drupal_add_js(drupal_get_path('module', 'os_modalframe').'/theme/os_modal_tabbed.js');

  return $form;
}

/**
 * Runs the links through theme().
 * We do it here because $box->options_form gets called in an access function
 */
function os_modal_tabbed_links_prerender($elements) {
  $elements['#value'] = theme('links', $elements['#value']);
  return $elements;
}

/**
 * After submission, the form has no idea how many tabs were added through js
 * I find out here and add new form elements for each new one.
 * This form element is placed before tabs so that the new elements go
 * through the builder properly. Removing a tab is handled in the submit.
 * Removing a tab and adding a new one should handle the same as editting an existing
 */
function os_modal_tabbed_add_new_tabs(&$element, $post, &$form_state, &$form) {
  for ($i=$element['#default_value']; $i < $post['tab_count']; $i++) {
    $form['tabs'][$i] = $form['tabs'][0];
    $form['tabs'][$i]['#attributes']['id'] = $i;
  }
  return $element;
}