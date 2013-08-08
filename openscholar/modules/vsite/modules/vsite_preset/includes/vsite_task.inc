<?php

/**
 * @file
 * Provides the vsite task base class which extends the class og_task.
 */

/**
 * Class vsite_task
 */
class vsite_task extends og_task {
  public $entity;
  public $entity_type;
  protected $active_space;

  /**
   * Constructor method.
   *
   * @param string $entity_type
   *   the entity type machine name, i.e. 'node'
   * @param object $entity
   *   the entity object instance, i.e. a vsite node object
   * @param string $title
   *   (Optional) the name of the task that displays in the checklist report
   * @param string $description
   *   (Optional) the description of the task for the checklist report
   */
  function __construct($entity_type, $entity, $title = NULL, $description = NULL) {
    $this->entity = $entity;
    $this->entity_type = $entity_type;

    if ($title) {
      $this->title = t($title);
    }

    if ($description) {
      $this->description = t($description);
    }

    $this->required = FALSE;
    if (!$entity->spaces_preset_og) {
      return;
    }
  }
}
