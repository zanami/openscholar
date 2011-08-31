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
    $node = node_load($this->options['nid']);

    $block = parent::render();
    $block['content'] = '<div id="logo"><a href="'.url("<front>").'">'.theme('vsite_logo',vsite_get_vsite(),'field_vsite_logo' ,'vsite_design_landscape_logo').'</a></div>';
    return $block;
  }

  public function form_alter(&$form, &$form_state) {
    if ($form['#id'] = 'node-form' && $form['form_id']['#value'] = 'vsite_node_form') {
      parent::form_alter($form, $form_state);

      // hide unnecessary fields
      dpm($form);
      $form['field_gallery']['#type'] = 'value';
      $form['options']['#type'] = 'value';
      $form['author']['#type'] = 'value';
      $form['book']['#type'] = 'value';
      $form['menu']['#type'] = 'value';
      $form['comment_settings']['#type'] = 'value';
      $form['path']['#type'] = 'value';
    }
  }
}