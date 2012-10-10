<?php

/**
 * Handle for file fields.
 */
class OSMigrateFileFieldHandler extends MigrateFileFieldHandler {
  public function __construct() {
    $this->registerTypes(array('file'));
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
      'og_group_ref' => t('Subfield: Groups this file belongs to'),
    );
    return $fields;
  }

  /**
   * Implementation of MigrateFileFieldBaseHandler::buildFieldArray().
   */
  protected function buildFieldArray($field_array, $arguments, $delta) {

    if (isset($arguments['og_group_ref'])) {
      if (is_array($arguments['og_group_ref'])) {
        $field_array['og_group_ref'] = $arguments['og_group_ref'][$delta];
      }
      else {
        $field_array['og_group_ref'] = $arguments['og_group_ref'];
      }
    }
    else {
      $field_array['og_group_ref'] = null;
    }

    return $field_array;
  }
}