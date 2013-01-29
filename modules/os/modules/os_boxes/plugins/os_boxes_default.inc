<?php

/**
 * @file
 * The base class for OS custom widgets. It defines an "abstract box"
 * to be extended by  "real boxes" like "listings", "feeds", "images" etc.
 *
 * It provides the widget visibility optoins. Users will decide where the
 * widget will be shown (i.e. in the current section / feature or everywhere in
 * the site
 *
 * One idea would be to even remove the widget visiblity options from here and
 * let the higher layers to deal with it
 */


abstract class os_boxes_default extends boxes_box {

  /**
   * If this box has been overridden the changed timestamp will be held here
   */
  public $changed;

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    return array();
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = array();

    $form['advanced'] = array(
      '#type' => 'fieldset',
      '#access' => user_access('use boxes advanced settings'),
      '#title' => t('Advanced Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 1000,
      '#after_build' => array('_os_boxes_default_advanced_handler'),
    );

    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $title = isset($this->title) ? $this->title : NULL;
    $block['title'] = $title;
    $block['delta'] = $this->delta;
    $block['content'] = '';
    return $block;
  }

  /**
   * Set the block in cache.
   *
   * @param $cid
   *   Array with options to be serialized to retrieve the cache ID.
   * @param $block
   *   The block object.
   */
  public function cache_set($cid, $block) {
    cache_set($this->prepareCid($cid), $block);
  }

  /**
   * Get the block in cache.
   *
   * @param $cid
   *   Array with options to be serialized to retrieve the cache ID.
   *
   * @return
   *   The block object or FALSE.
   */
  public function cache_get($cid) {
    $cache = cache_get($this->prepareCid($cid));
    return !empty($cache->data) ? $cache->data : FALSE;
  }

  /**
   * Return the cache ID string, by prefix it with group ID, if present.
   *
   * @param $cid
   *   Array with options to be serialized to retrieve the cache ID.
   *
   */
  private function prepareCid($cid) {
    $new_cid = 'os_boxes:';
    if (module_exists('og_context') && $og_context = og_context()) {
      $new_cid .= $og_context['gid'] . ':';
    }

    $new_cid .= md5(serialize($cid));
    return $new_cid;
  }
}

function _os_boxes_default_advanced_handler($element, $form_state) {
  $children = element_children($element);
  if (empty($children)) {
    $element['#access'] = false;
  }

  return $element;
}
