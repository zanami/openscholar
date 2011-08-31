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
      $whitelist = array('field_vsite_logo');
      foreach (element_children($form) as $child) {
        if ($form[$child]['#type'] != 'value' && $form[$child]['#type'] != 'hidden') {
          if (!in_array($child, $whitelist)) {
            $form[$child]['#type'] = 'value';
          }
        }
      }
      /*
      $form['field_vsite_address']['#type'] = 'value';
      $form['options']['#type'] = 'value';
      $form['revision_information']['#type'] = 'value';
      $form['author']['#type'] = 'value';
      $form['book']['#type'] = 'value';
      $form['menu']['#type'] = 'value';
      $form['comment_settings']['#type'] = 'value';
      $form['path']['#type'] = 'value';
      $form['themes']['#type'] = 'value';
      $form['taxonomy']['#type'] = 'value';
      $form['og_description']['#type'] = 'value';
      $form['og_selective']['#type'] = 'value';
      $form['og_registry']['#type'] = 'value';
      $form['og_directory']['#type'] = 'value';
      $form['attachments']['#type'] = 'value';
      $form['title']['#type'] = 'value';
*/
      $form['buttons']['#type'] = 'markup';
      $form['buttons']['preview']['#access'] = false;
      $form['buttons']['delete']['#access'] = false;
      $form['buttons']['preview_changes']['#access'] = false;
    }
  }
}