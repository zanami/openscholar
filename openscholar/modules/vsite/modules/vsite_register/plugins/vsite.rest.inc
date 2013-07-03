<?php

/**
 * @file
 * Defines a RESTws Resource for the Vsite object.
 */

class VsiteResourceController implements RestWSQueryResourceControllerInterface {

  public function __construct($name, $info) {
    $this->entityType = 'node';
  }

  public function propertyInfo() {
    return array(
      'properties' => array(
        'title' => array(
          'type' => 'text',
          'label' => t('Site title'),
          'setter callback' => 'entity_property_verbatim_set',
        ),
        'domain' => array(
          'type' => 'text',
          'label' => t('Domain'),
          'setter callback' => 'entity_property_verbatim_set',
        ),
        'purl' => array(
          'type' => 'text',
          'label' => t('Site PURL'),
          'setter callback' => 'entity_property_verbatim_set',
        ),
        'type' => array(
          'type' => 'text',
          'label' => t('Site Type'),
          'setter callback' => 'entity_property_verbatim_set',
        ),
        'preset' => array(
          'type' => 'text',
          'label' => t('Preset'),
          'setter callback' => 'entity_property_verbatim_set',
        ),
        'visiblity' => array(
          'type' => 'int',
          'label' => t('Visibility'),
          'setter callback' => 'entity_property_verbatim_set',
        ),
        'owner' => array(
          'type' => 'int',
          'label' => t('Site Owner'),
          'setter callback' => 'entity_property_verbatim_set',
        )
      ),
    );
  }

  public function access($op, $id) {
    return true;
  }

  public function read($id) {

  }

  public function create(array $values) {
   if ($values['owner'] == -1) {
      $values['owner'] = $GLOBALS['user']->uid;
    }
    elseif ($values['owner'] == 0) {
      // create or find new user
      // assign them as owner of new site
    }

    /*$vsite = vsite_create_vsite($values['title'],
                      $values['domain'],
                      $values['owner'],
                      $values['type'],
                      $values['preset'],
                      NULL,
                      $values['visibility']);

    return $vsite->group->nid; */
  }

  public function update($id, array $values) {

  }

  public function delete($id) {

  }

  public function wrapper($id) {
    return entity_metadata_wrapper('node', $id);
  }

  public function resource() {
    return 'vsite';
  }

  public function query($filters = array(), $meta_controls = array()) {

  }

  public function count($filters = array(), $meta_controls = array()) {
    return 10;
  }

  public function limit($client_limit = NULL) {
    return 10;
  }
}