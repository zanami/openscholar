<?php

/**
 * @file
 * Imported entity fetcher for the feeds module.
 */
class ImportedEntityFetcher extends FeedsProcessor {

  /**
   * Define entity type.
   */
  public function entityType() {
    return 'imported_entity';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Imported entities');
    return $info;
  }

  /**
   * Creates a new imported entity and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $type = $this->config['imported_entity'];
    $imported_entity = imported_entity_create($type);
    return $imported_entity;
  }

  /**
   * Loads an existing imported entity.
   */
  protected function entityLoad(FeedsSource $source, $id) {
    $imported_entity = imported_entity_load($id);
    return $imported_entity;
  }

  /**
   * Save a imported entity.
   */
  protected function entitySave($entity) {
    $entity->save();
  }

  /**
   * Delete multiple user accounts.
   */
  protected function entityDeleteMultiple($ids) {
    imported_entity_delete_multiple($ids);
  }

  public function configDefaults() {
    return array(
      'type' => array(),
      'mappings' => array(),
    );
  }

  public function configForm(&$form_state) {
    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Source type'),
      '#description' => t('Select the source type you will migrate from.'),
      '#options' => array(
        'xml' => t('XML'),
        'json' => t('JSON'),
      ),
      '#default_value' => $this->config['type'],
    );
    return $form;
  }

  public function getMappingTargets() {
    self::loadMappers();

    $targets = array(
      'title' => array(
        'name' => t('Title'),
      ),
      'description' => array(
        'name' => t('Description'),
      ),
      'image' => array(
        'name' => t('Image'),
      ),
      'terms' => array(
        'name' => t('Terms'),
      ),
    );

    return $targets;
  }
}
