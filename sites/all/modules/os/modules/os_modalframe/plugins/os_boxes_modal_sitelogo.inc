<?php

class os_boxes_modal_sitelogo extends os_boxes_modalframe {

  protected function config_path() {
    $vsite = vsite_get_vsite();
    return 'node/'.$vsite->group->nid.'/edit';
  }

  public function options_defaults() {
    return array();
  }

  public function render() {
    $vsite = vsite_get_vsite();

    $block = parent::render();

    if ($vsite->group->type == 'vsite') {
      $block = array_merge($block, _scholar_logo_wdgt());
    }
    elseif ($vsite->group->type == 'project') {
      $block = array_merge($block, _scholar_project_logo_wdgt());
    }
    return $block;
  }

  public function form_alter(&$form, &$form_state) {
    if ($form['#id'] = 'node-form' && $form['form_id']['#value'] = 'vsite_node_form') {
      parent::form_alter($form, $form_state);

      // hide unnecessary fields
      $whitelist = array('field_vsite_logo', 'field_project_logo');
      foreach (element_children($form) as $child) {
        if ($form[$child]['#type'] != 'value' && $form[$child]['#type'] != 'hidden') {
          if (!in_array($child, $whitelist)) {
            $form[$child]['#type'] = 'value';
          }
        }
      }

      $form['buttons']['#type'] = 'markup';
      $form['buttons']['preview']['#access'] = false;
      $form['buttons']['delete']['#access'] = false;
      $form['buttons']['preview_changes']['#access'] = false;
    }
  }
}