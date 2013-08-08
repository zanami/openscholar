<?php

/**
 * @file
 * OS feed items fetcher for the feeds module.
 */
class OsFeedReaderFetcher extends FeedsProcessor {

  /**
   * Define entity type.
   */
  public function entityType() {
    return 'os_feed_item';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('OS feed items');
    return $info;
  }

  /**
   * Creates a new imported entity and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $os_reader = os_reader_create();
    return $os_reader;
  }

  /**
   * Loads an existing imported entity.
   */
  protected function entityLoad(FeedsSource $source, $id) {
    $os_reader = os_reader_load($id);
    return $os_reader;
  }

  /**
   * Save a imported entity.
   */
  protected function entitySave($entity) {
    $file = system_retrieve_file($entity->image, NULL, TRUE);
    $values = array(
      'title' => $entity->title,
      'description' => $entity->description,
      'created' => strtotime($entity->created),
      'fid' => $file->fid,
    );
    os_reader_create($values)->save();
  }

  /**
   * Delete multiple user accounts.
   */
  protected function entityDeleteMultiple($ids) {
    os_reader_delete_multiple($ids);
  }

  public function configDefaults() {
    return array(
      'type' => array(),
      'mappings' => array(),
    );
  }

  function configForm(&$form_state) {
    $form = array();
    return $form;
  }

  public function getMappingTargets() {
    $targets = array(
      'title' => array(
        'name' => t('Title'),
        'optional_unique' => TRUE,
      ),
      'description' => array(
        'name' => t('Description'),
      ),
      'image' => array(
        'name' => t('Image'),
      ),
      'created' => array(
        'name' => t('Published date'),
      ),
      'url' => array(
        'name' => t('URL'),
      ),
    );

    self::loadMappers();
    $entity_type = $this->entityType();
    $bundle = 'os_feed_item';
    drupal_alter('feeds_processor_targets', $targets, $entity_type, $bundle);

    return $targets;
  }

  /**
   * Prevent duplication of entries.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    if ($id = parent::existingEntityId($source, $result)) {
      return $id;
    }

    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      switch($target) {
        case 'title':
          $query = new entityFieldQuery();
          $result = $query
            ->entityCondition('entity_type', 'os_feed_item')
            ->propertyCondition('title', $value)
            ->execute();

          if (!empty($result['os_feed_item'])) {
            $id = reset(array_keys($result['os_feed_item']));
          }
          break;
      }
    }

    // Return the identifier of the entity.
    if ($id) {
      return $id;
    }

    return 0;
  }
}
