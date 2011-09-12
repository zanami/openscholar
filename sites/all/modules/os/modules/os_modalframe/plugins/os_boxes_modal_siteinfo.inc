<?php

class os_boxes_modal_siteinfo extends os_boxes_modalframe {

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
      $block = array_merge($block, _scholar_basicinfo_wdgt);
    }
    elseif ($vsite->group->type == 'project') {
      $block = array_merge($block, _scholar_project_basicinfo_wdgt());
    }
/*
    if (!$vsite->group->title) {
      $cprofile = content_profile_load('vsite_users_profile', $vsite->group->uid);

      $headline = (is_array($cprofile->vsite_users_last_name))? $cprofile->vsite_users_first_name[0]['value']." ".$cprofile->vsite_users_last_name[0]['value']:"";
      $subheadline = (is_array($cprofile->vsite_users_title))?  $cprofile->vsite_users_title[0]['value']:"";

      $headline_markup = (strlen($headline) > 2) ?  "<h1>" . l($headline, $vsite->get_absolute_url()). "</h1>"  : "";
      $subheadline_markup =  strlen($subheadline) ?  "<h2>" . $subheadline . "</h2>" : "";
      $p_tags = (strlen($headline_markup) || strlen($subheadline_markup)) ? "<p></p>" : "";

      $block['content'] = $headline_markup . $subheadline_markup . $p_tags;
    }
    else {
      $contact_anon = variable_get('vsite_users_contact_form_anonymous', TRUE);

      $s_email = ($contact_anon !== 0 || user_is_logged_in())?l("(email)","contact_owner"):"";
      $s_address = count($vsite->group->field_vsite_address)?$vsite->group->field_vsite_address[0]['value']:"";

      $output =   "<h1>" . l($vsite->group->title, $vsite->get_absolute_url()) ."</h1>" .
                          (strlen($vsite->group->og_description) ? "<h2>" . $vsite->group->og_description. "</h2>" : "") .
                          " <p>{$s_address}&nbsp;{$s_email}</p>";

      $block['content'] = $output;
    }
*/
    return $block;
  }

  public function form_alter(&$form, &$form_state) {
    if ($form['#id'] = 'node-form' && $form['form_id']['#value'] = 'vsite_node_form') {
      parent::form_alter($form, $form_state);

      // hide unnecessary fields
      $whitelist = array('title', 'og_description', 'field_vsite_address');
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

  static public function submit($form, $form_state) {

  }
}