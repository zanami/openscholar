<?php

/**
 * OG vocab behavior handler.
 */
class OgVocabBehaviorHandler extends EntityReference_BehaviorHandler_Abstract {

  public function access($field, $instance) {
    return $field['field_name'] == OG_VOCAB_FIELD;
  }

  public function is_empty_alter(&$empty, $item, $field) {
    if (!empty($item['target_id']) && $item['target_id'] == 'autocreate') {
      $empty = FALSE;
    }
  }

  public function presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
    foreach ($items as $delta => $item) {
      if ($item['target_id'] == 'autocreate') {
        $term = (object) $item;
        unset($term->tid);
        taxonomy_term_save($term);
        $items[$delta]['target_id'] = $term->tid;
      }
    }
  }
}
