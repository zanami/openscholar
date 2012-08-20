<?php

class os_boxes_tabs extends os_boxes_default {

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    $options['qt_obj'] = array(
      'default_tab' => FALSE,
      'tabs' => array(),
    );
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);

    if (!module_exists('quicktabs')) {
      $form['warning'] = array('#markup' => t('This widget requires the QuickTabs module to be enabled.'));
      return $form;
    }

    $form['#attached'] = array(
      'library' => array(
        array('system', 'ui.tabs'),
      ),
      'js' => array(
        drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_tabs/os_boxes_tabs_form.js',
      ),
      'css' => array(
        drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_tabs/os_boxes_tabs.css',
      ),
    );

    $widgets = module_invoke_all('os_widget');

    // filter out factories
    $options = array('os_boxes_tabs-remove' => t('No Widget'));
    ctools_include('layout', 'os');
    foreach ($widgets as $wgt => $info) {
      $options[$wgt] = $info['info'];  // not everything we get back from hook_os_widget has an info field.
      // This needs to be rectified somehow.
    }

    $qt = (array)$this->options['qt_obj'];
    $existing = $qt['tabs'];
    if (!count($existing)) {
      $existing[] = array(
        'type' => 'block',
        'title' => t('First Tab'),
        'weight' => 0,
        'delta' => 'os_boxes_tabs-remove',
        'bid' => 'first-tab',
      );
    }
    $new_tab = array(
      'type' => 'block',
      'title' => 'New Tab',
      'weight' => '200000 new',  // special case
      'delta' => 'os_boxes_tabs-remove',
      'bid' => '__blank__',
    );
    $tabs_data = array_merge($existing, array('new' => $new_tab));

    $links = array();

    $form['tabs'] = array(
      '#type' => 'container',
      '#attributes' => array('id' => 'tabs'),
      '#tree' => TRUE,
      '#description' => t('Drag the tabs to reorder them. Whichever tab is on the left will be the open tab when a user visits the page.'),
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
        '#attributes' => array('id' => $id, 'class' => array('panel')),
        '#weight' => 1,
        'bid' => array(
          '#type' => 'select',
          '#title' => t('Widget to Display in this Tab'),
          '#options' => $options,
          '#default_value' => $t['bid'],
          '#description' => t('Choose which widget to display in this tab. Select \'No Widget\' to remove this tab.'),
          '#attributes' => array('class' => array('tab-delta')),
        ),
        'title' => array(
          '#title' => t('Title of the Tab'),
          '#type' => 'textfield',
          '#default_value' => $t['title'],
          '#attributes' => array('class' => array('tab-title')), // for js
        ),
        'weight' => array(
          '#type' => 'hidden',
          '#default_value' => $t['weight'],
          '#attributes' => array('class' => array('tab-weight')),  // for js
        ),
        'type' => array(
          '#type' => 'value',
          '#value' => 'block',
        ),
      );
    }

    ksort($links);
    $form['tabs']['links'] = array(
      '#theme' => 'links',
      '#links' => $links,
      '#attributes' => array(
        'class' => array('links'),
      ),
      '#weight' => 0,
    );

    // stores the number of real tabs
    // as soon as user picks a widget, this is incremented with js
    $form['tab_count'] = array(
      '#type' => 'hidden',
      '#default_value' => count($existing),
      '#process' => array('os_modal_tabbed_add_new_tabs'),
      '#attributes' => array(
        'id' => 'edit-count',
      ),
    );

    return $form;
  }

  public function options_submit(&$form, &$form_state) {
    $vals = &$form_state['values'];
    dpm($vals);

    $qt = array(
      'machine_name' => strtolower(str_replace(' ', '_', $vals['description'])),
      'ajax' => 0,
      'hide_empty_tabs' => 0,
      'title' => $vals['description'],
      'tabs' => array(),
    );

    foreach ($vals['tabs'] as $k => $t) {
      if ($k == 'tab-new' || $t['bid'] == 'os_boxes_tabs-remove') continue;
      $qt['tabs'][$t['weight']] = $t;
    }

    ksort($qt['tabs']);
    reset($qt['tabs']);
    $qt['default_tab'] = key($qt['tabs']);
    $vals['box']['qt_obj'] = $qt;
    $vals['box']['title'] = $vals['title'];
    $vals['box']['description'] = $vals['description'];

    return parent::submit($form, $form_state);
  }

  /**
   * Implementation of render().
   */
  public function render() {
    $block = parent::render();
    if (!module_exists('quicktabs')) {
      $block['content'] = t('This widget requires the QuickTabs module to be enabled.');
      return $block;
    }

    $plugin = context_get_plugin('reaction', 'block');
    $qt = $this->options['qt_obj'];
    foreach ($qt['tabs'] as $k => &$t) {
      if ($b = $plugin->get_block($t['bid'])) {
        $b->subject = '';
        if (empty($b->content)) {
          unset($qt['tabs'][$k]);
        }
        else {
          if ($block_arr = _block_render_blocks(array($block))) {
            if (!empty($block_arr["{$block->module}_{$block->delta}"]->content)) {
              $build = _block_get_renderable_array($block_arr);
              $t['content'] = $build;
            }
          }
        }
      }
      else {
        unset($qt['tabs'][$k]);
      }
    }
    $qt_obj = quicktabs_build_quicktabs('os_boxes_quicktabs-'.$this->delta, $qt);
    if (isset($qt_obj['content']) && !empty($qt_obj['content'])) {
      $block['content'] = drupal_render($qt_obj['content']);
      drupal_add_js(drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_tabs/os_modal_tabbed_display.js');
    }

    return $block;
  }
}

/**
 * After submission, the form has no idea how many tabs were added through js
 * I find out here and add new form elements for each new one.
 * This form element is placed before tabs so that the new elements go
 * through the builder properly. Removing a tab is handled in the submit.
 * Removing a tab and adding a new one should handle the same as editting an existing
 */
function os_modal_tabbed_add_new_tabs(&$element, &$form_state, &$form) {
  for ($i=$element['#default_value']; $i < $element['#value']; $i++) {
    $new_tab = 'tab-'.$i;
    $form['options']['tabs'][$new_tab] = $form['options']['tabs']['tab-new'];
    $form['options']['tabs'][$new_tab] += array(
      '#attributes' => array('id' => $i),
      '#parents' => array('tabs', $new_tab),
      '#array_parents' => array('options', 'tabs', $new_tab),
      '#tree' => TRUE,
    );
    $form['options']['tabs'][$i] += $form['options']['tabs']['blank'];
    foreach ($form['options']['tabs'][$i] as $key => &$child) {
      if (strpos($key, '#') !== 0) {
        unset($child['#parents']);
        unset($child['#name']);
        unset($child['#value']);
      }
    }
    form_builder($form['form_id']['#value'], $form['options']['tabs'][$new_tab], $form_state);
  }
  unset($form_state['values']['tabs']['blank']);

  // remove nids that aren't in the post
  if (is_array($form_state['values'])) {
    foreach ($form_state['values']['tabs'] as $k => $v) {
      if (!isset($form_state['input']['tabs'][$k])) {
        unset($form_state['values']['tabs'][$k]);
      }
    }
  }
  return $element;
}