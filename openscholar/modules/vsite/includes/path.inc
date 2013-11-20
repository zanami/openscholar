<?php
// $Id$


/**
 * Return the pathauto settings modified to work inside vsites
 */
function _vsite_get_pathauto_settings() {
  $settings = array();

  // Prepends vsite purl to all group content node patterns.
  $bundles = array_keys(os_get_bundles(TRUE));
  $entity = 'node';
  foreach ($bundles as $bundle) {
    $pattern = pathauto_pattern_load_by_entity($entity, $bundle);
    $settings["pathauto_{$entity}_{$bundle}_pattern"] = "[vsite:site-purl]/" . $pattern;
  }
  // Prepends vsite purl to taxonomy term pattern.
  $pattern = pathauto_pattern_load_by_entity('taxonomy_term');
  $settings["pathauto_taxonomy_term_pattern"] = "[vsite:site-purl]/" . $pattern;

  return $settings;
}

function _vsite_alter_node_path_settings(&$node) {
  $vsite = vsite_get_vsite();

  if (empty($vsite)) {
    // Currently not inside a vsite but might have vsite context e.g Migration.
    $wrapper = entity_metadata_wrapper('node', $node);
    $gid = !empty($wrapper->{OG_AUDIENCE_FIELD}) ? $wrapper->{OG_AUDIENCE_FIELD}->get(0)->getIdentifier() : NULL;
    $vsite = !empty($gid) ? vsite_get_vsite($gid) : NULL;

    if (empty($vsite)) {
      // No vsite context.
      return;
    }
  }

  if ((int) $node->nid === (int) $vsite->group->nid) {
    // This is the group node, it doesn't need a prefix.
    return;
  }

  // Get purl.
  $purl = vsite_get_purl($vsite);

  // Get alias pattern for pathauto.
  module_load_include('inc', 'pathauto');
  $pattern = pathauto_pattern_load_by_entity('node', $node->type, $node->language);

  if ($pattern && (!(isset($node->path['pathauto']) && empty($node->path['pathauto'])))) {
    // Generate alias using pathauto.

    // Generates the pathauto alias without purl.
    $alias = _vsite_alias_without_purl($node);
    // Add purl prefix to alias.
    $alias = $purl . '/' . $alias;

    // Aborts pathauto for this node if the path already exists.
    if (_pathauto_path_is_callback($alias)) {
      $node->path['pathauto'] = 0;
      return;
    }
  }
  // Note that Drupal has already validated the path prior to hook_node_presave,
  // so there's no way that an existing path has gotten to this point.
  else {
    $alias = ltrim($node->path['alias'], '/');

    if (empty($alias)) {
      // No alias.
      return;
    }

    if (!empty($purl) && (strpos($alias, $purl) !== 0)) {
      // Add purl to alias.
      $node->path['alias'] = $purl . '/' . $alias;
    }
  }
}

/**
 * Integrates pathauto & purl for taxonomy terms.
 *
 * @see vsite_taxonomy_term_presave()
 * @see _vsite_alter_node_path_settings()
 */
function _vsite_alter_term_path_settings(&$term) {
  if (!$vsite = vsite_get_vsite()) {
    return;
  }
  module_load_include('inc', 'pathauto');
  $pattern = pathauto_pattern_load_by_entity('taxonomy_term');

  // For pathauto generated aliases, verify no naming collisions
  // Only checks aliases if a pathauto pattern exists and pathauto will alias.
  if ($pattern && (!(isset($term->path['pathauto']) && empty($term->path['pathauto'])))) {
    // Generates the pathauto alias and removes pURL modifiers
    $alias = _vsite_alias_without_purl($term, 'taxonomy_term');
    // Aborts pathauto for this node if the path already exists.
    if (_pathauto_path_is_callback($alias)) {
      $term->path['pathauto'] = 0;
      return;
    }
  }

  // For manually entered aliases, prefix alias with the purl modifier.
  // Note that Drupal has already validated the path prior to hook_node_presave,
  // so there's no way that an existing path has gotten to this point.
  else {
    $path = ltrim($term->path['alias'], "/");
    // Is there a valid path provided by the user?
    if (strlen($path) === 0) {
      return;
    }
    $no_purl_prefix = (strlen($vsite->group->purl) && (strpos($path, $vsite->group->purl . "/") !== 0));
    if ($no_purl_prefix) {
      $term->path['alias'] = $vsite->group->purl . "/" . $path;
    }
  }
}

/**
 * Returns the pathauto alias for a node, with any pURL modifiers removed.
 *
 * @param object $node
 *
 * @return string $alias
 *
 * @see _vsite_alter_node_path_settings()
 */
function _vsite_alias_without_purl($entity, $entity_type = 'node') {
  $uri      = entity_uri($entity_type, $entity);
  $language = !empty($entity->language) ? $entity->language : LANGUAGE_NONE;
  $bundle   = isset($entity->type) ? $entity->type : '';
  $alias    = pathauto_create_alias($entity_type, 'return', $uri['path'], array($entity_type => $entity), $bundle, $language);

  // Removes purl elements from alias string.
  $processor = purl_get_processor(PURL_VSITE_PATH);
  $elements = purl_parse($processor, $alias);
  foreach ($elements as $element) {
    $alias = $processor->remove($alias, $element);
  }

  return $alias;
}
