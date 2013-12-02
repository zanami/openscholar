<?php

/**
 * @file
 * Provides class for os_boxes_tabs.
 */

/**
 * Provides class for os_boxes_tabs.
 */
class os_boxes_tabs extends os_boxes_widget_collection {

  /**
   * Implements render().
   */
  public function render() {
    $block = parent::render();

    $plugin = context_get_plugin('reaction', 'block');
    $blocks = $plugin->get_blocks();
    $render = array(
      'links' => array(
        '#theme' => 'links',
        '#links' => array(),
      ),
      '#attached' => array(
        'library' => array(
          array('system', 'ui.tabs'),
        ),
        'js' => array(
          drupal_get_path('module', 'os_boxes') . '/plugins/os_boxes_tabs/os_boxes_tabs_display.js',
        ),
      ),
    );
    module_load_include('module', 'block', 'block');
    foreach ($this->options['tabs'] as $k => & $tab) {
      if ($block_tab = $blocks[$tab['bid']]) {
        if ($block_arr = _block_render_blocks(array($block_tab))) {
          if (!empty($block_arr["{$block_tab->module}_{$block_tab->delta}"]->content)) {
            $block_obj = &$block_arr[$block_tab->module . '_' . $block_tab->delta];
            $block_obj->region = $this->delta;

            $render['links']['#links'][$tab['weight']] = array(
              'title' => $tab['title'],
              'href' => '',
              'fragment' => $k,
              'external' => TRUE,
            );

            $build = _block_get_renderable_array($block_arr);
            $build['#attributes']['id'] = '';
            $render[$tab['bid']] = array(
              '#type' => 'container',
              '#attributes' => array('id' => $k),
              $tab['bid'] => $build,
            );
          }
        }
      }
    }
    ksort($render['links']['#links']);
    $block['content'] = drupal_render($render);

    return $block;
  }
}

/**
 * Includes new (dynamically added) tabs in form processing.
 *
 * After submission, the form has no idea how many tabs were added through js
 * I find out here and add new form elements for each new one.
 * This form element is placed before tabs so that the new elements go
 * through the builder properly. Removing a tab is handled in the submit.
 * Removing a tab and adding a new one should handle the same as editting an existing
 */
function os_modal_tabbed_add_new_tabs(&$element, &$form_state, &$form) {
  for ($i = $element['#default_value']; $i < $element['#value']; $i++) {
    $new_tab = 'tab-' . $i;
    $form['options']['tabs'][$new_tab] = array(
      '#attributes' => array('id' => $i),
      '#parents' => array('tabs', $new_tab),
      '#array_parents' => array('options', 'tabs', $new_tab),
      '#tree' => TRUE,
    );
    $form['options']['tabs'][$new_tab] += $form['options']['tabs']['tab-new'];
    foreach ($form['options']['tabs'][$new_tab] as $key => & $child) {
      if (strpos($key, '#') !== 0) {
        unset($child['#parents']);
        unset($child['#name']);
        unset($child['#value']);
      }
    }
    form_builder($form['form_id']['#value'], $form['options']['tabs'][$new_tab], $form_state);
  }
  unset($form_state['values']['tabs']['tab-new']);

  // Removes nids that aren't in the post
  if (is_array($form_state['values'])) {
    foreach ($form_state['values']['tabs'] as $k => $v) {
      if (!isset($form_state['input']['tabs'][$k]) || !isset($form_state['input']['tabs'][$k]['bid']) || $form_state['input']['tabs'][$k]['bid'] == 'os_boxes_tabs-remove') {
        unset($form_state['values']['tabs'][$k]);
      }
    }
  }
  return $element;
}

