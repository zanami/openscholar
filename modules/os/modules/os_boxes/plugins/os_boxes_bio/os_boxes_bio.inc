<?php
// $Id$

/**
 * @file
 * Provides class for "Bio" type widgets.
 */

/**
 * Displays the Bio node if exists, provides display options and empty text.
 *
 * @see OS branch 2.0-RC1 ./sites/all/modules/os/modules/os_boxes/plugins/os_boxes_bio/os_boxes_bio.inc
 */
class os_boxes_bio extends os_boxes_default {

  /**
   * Implements boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    return $options;
  }

  /**
   * Implements boxes_content::options_form().
   */
  public function options_form() {

    // Exits if we are not in a vsite
    // @todo  2012-09-12 Why? -oren
    //    if (($vsite = vsite_get_vsite()) == FALSE) {
    //      return array();
    //    }

    $default_nid = $this->options['nid'];

    $form += parent::options_form();
    $form['nid']['#type'] = 'value';
    $form['nid']['#value'] = 0;
    unset($form['nid']['#element_validate']);

    $form['teaser'] = array(
      '#type' => 'radios',
      '#title' => 'Display Mode',
      '#default_value' => $this->options['teaser'],
      '#options' => array('teaser' => 'Teaser Only', 'full' => 'Full Bio'),
      '#description' => 'Display your entire Bio in this widget, or just the teaser.',
    );

    $form['show_title'] = array(
      '#type' => 'checkbox',
      '#title' => 'Display title of your Bio',
      '#default_value' => $this->options['show_title'],
    );

    return $form;
  }

  /**
   * Implements boxes_content::render().
   */
  public function render() {

    // Exits if we are not in a vsite
    if (($vsite = vsite_get_vsite()) == FALSE) {
      return array();
    }

    // Automatically fills in Bio node ID if available.
    $node = _os_biocv_get_bio_node();
    if ($node !== FALSE) {
      $this->options['nid'] = $node->nid;
      $block = parent::render();

      if (($vsite = vsite_get_vsite()) == FALSE) {
        return $block;
      }

      $nid = $this->options['nid'];

      if ((int)$nid !== 0) {
        $node = node_load($nid);
      }

      if ($node) {
        $teaser = ($this->options['teaser'] == 'teaser') ? TRUE : FALSE;

        if (isset($this->options['show_title']) && !$this->options['show_title']) {
          // Hide the nide title
          $node->title = '';
        }

        $output = node_view($node, $teaser, FALSE, FALSE);
        $block['content'] = $output;
      }
    }
    // Renders empty text if no bio exists yet.
    else {
      $block['content'] = $this->render_no_bio();
    }

    return $block;
  }

  /**
   * Renders empty text for a vsite admin without a Bio node
   */
  protected function render_no_bio() {

    $spaces_features = variable_get('spaces_features', array());

    // If the bio is disabled show nothing
    if (!$spaces_features["os_biocv"]) {
      return '';
    }

    // Otherwise, displays vsite admin empty text, prompting bio configuration.
    $wrapper = array(
      '#type' => 'container',
      '#attributes' => array(
        'id' => array('os-boxes-bio-empty-text'),
      ),
    );

    $wrapper['empty_text'] = array(
      '#type' => 'markup',
      '#markup' => t('Your sites front page is set to display your bio by default. @create_bio or @edit_layout for the front page of your site.',
        array(
          '@create_bio' => l(t('Create your bio now'), '/node/add/bio'),
          '@edit_layout' => l(t('change what displays'), '/cp/build/layout', array('query' => array('page_type' => 'front'))),
        )
      ),
    );

    return render($wrapper);
  }

  /**
   * Returns the cache parameters for this box
   */
  public function cache_info() {

    // Clears when bio nodes are saved
    $cache_key = array(
      'os_og' => array('current' => 'current'),
      'node' => array('bio' => 'bio'),
    );

    return array('type' => BLOCK_CACHE_CONTENT_CACHE, 'keys' => $cache_key);
  }
}

