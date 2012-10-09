<?php

/**
 * @file imagefield_crop.inc
 *
 * Migration field handler for imagefield_crop
 *
 * @author sagotsky
 *
 */
class MigrateImagefieldcropFieldHandler extends MigrateImageFieldHandler {
  public function __construct() {
    // This handler will take care of all the simple core field types
    $this->registerTypes(array('imagefield_crop'));
  }

  // Here, we take a simple array of values (for an integer field, it might look like
  // array(35, -23, 85738)
  public function prepare($entity, array $field_info, array $instance, array $values, $langcode=LANGUAGE_NEGOTIATION_DEFAULT) {
    //crop the image.
    if (isset($values[0]) && $values[0] && (int)$values[0] > 0) {
      $items = array('fid' => $values[0]); //also has cropbox_x and y widht and height
      $items += array_intersect_key($values['arguments'], array_flip(array('cropbox_x', 'cropbox_y', 'cropbox_width', 'cropbox_height')));
      $items = array($items);
      imagefield_crop_field_insert('node', $entity, $field_info, $instance, $langcode, $items);
      //now what?
    }

    $return = parent::prepare($entity, $field_info, $instance, $values);
    if (!count($return)) {
      return $return;
    }

    $migration = Migration::currentMigration();
    $arguments = (isset($values['arguments']))? $values['arguments']: array();
    unset($values['arguments']);
    // In Drupal 7, field values are indexed by language, which can be specified
    // at various levels - this member implemented in MigrateFieldHandler works
    // out which language to use, applying appropriate defaults
    $language = $this->getFieldLanguage($entity, $field_info, $arguments);
    // Setup the standard Field API array for saving.
    $delta = 0;
    foreach ($values as $value) {
      if ($value) {
        $return[$language][$delta]['value'] = $value;
        $delta++;
      }
    }
    if (!isset($return)) {
      $return = NULL;
    }
    return $return;
  }


  /**
   * Implementation of MigrateFileFieldBaseHandler::buildFieldArray().
   */
  protected function buildFieldArray($field_array, $arguments, $delta) {
    foreach (array('cropbox_x', 'cropbox_y', 'cropbox_height', 'cropbox_width') as $field) {
      if (isset($arguments[$field])) {
        if (is_array($arguments[$field])) {
          $field_array[$field] = $arguments[$field][$delta];
        }
        else {
          $field_array[$field] = $arguments[$field];
        }
      }
    }

    return parent::buildFieldArray($field_array, $arguments, $delta);
  }

  /**
   * Implementation of MigrateFieldHandler::fields().
   * Note that file and image fields support slightly different field lists.
   *
   * @param $type
   *  The file field type - 'file' or 'image'
   * @param $parent_field
   *  Name of the parent field.
   * @param Migration $migration
   *  The migration context for the parent field. We can look at the mappings
   *  and determine which subfields are relevant.
   * @return array
   */
  public function fields($type, $parent_field, $migration = NULL) {
    $fields = parent::fields($type, $parent_field, $migration);
    $fields += array(
      'cropbox_x' => t('Horizontal origin of crop.'),
      'cropbox_y' => t('Vertical origin of crop.'),
      'cropbox_height' => t('Height of selected section of image.'),
      'cropbox_width' => t('Width of selected section of image.'),
    );

    return $fields;
  }
}

