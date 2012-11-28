<?php

/**
 * @file
 * The class for Harvard courses filter.
 */
class harvard_courses_filter extends os_boxes_default {

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state){
    $form = parent::options_form($form_state);
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    if (!$space = spaces_get_space()) {
      return;
    }

    $query = db_select('og_membership', 'ogm');
    $query->join('node', 'n', 'n.nid = ogm.etid');
    $results = $query
      ->fields('n')
      ->condition('ogm.gid', $space->id)
      ->condition('n.type', 'harvard_course')
      ->execute()
      ->fetchAllKeyed();

    if (empty($results)) {
      return;
    }

    $nodes = node_load_multiple(array_keys($results));
    $semester = array();
    foreach ($nodes as $node) {
      $wrapper = entity_metadata_wrapper('node', $node);
      $text = $wrapper->field_calendar_period->value();
      $semester[$text] = l($text, 'courses/' . check_plain($text));
    }

    $block = array(
      'delta' => 'harvard_courses_filter',
      'content' => theme('item_list', array('title' => t('Filter by semester'), 'items' => $semester)),
    );

    return $block;
  }
}
