<?php

/**
 * @file
 * The class for "Edit/Display Node" type widgets.
 */

class os_boxes_modalframe extends os_boxes_default {
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    $options['nid'] = 0;
    $options['type'] = '';
    $options['fields'] = '*';

    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   * Three cases:
   * 		nid is non-zero: We're editing this node/box
   * 					(get here from box instance configure link)
   *		type is not empty: We're adding a new node/box of the given type
   *					(get here from box factory configure link)
   *		both are empty: We're adding a new node/box of unknown type
   *					(get here from box factory configure link)
   */
  public function options_form(){
    //@todo just for vsite for now but it can be used outside os
    $form['nid'] = array(
      '#type' => 'value',
      '#value' => $this->options['nid'],
    );

    ctools_include('form');
    if ($this->options['nid']) {
      // we know what node we want already
      $node = node_load($this->options['nid']);
      // get the node form
      $form_state = array(
        'args' => array($node),
        'want form' => 1,
      );
      $node_form = ctools_build_form($node->type.'_node_form', $form_state);
      // remove unwanted elements
    }
    elseif (in_array($this->options['type'], content_types())) {
      // we have a content type, get the node form for it

      $node = array('uid' => $user->uid, 'name' => (isset($user->name) ? $user->name : ''), 'type' => $type, 'language' => '');
      $node_form = ctools_build_form($type .'_node_form', array('args' => array($node)));
      // remove unwanted elements
    }
    else {
      // we have neither
      // set up a content types dropdown and AHAH to go with it
    }
    $form += parent::options_form();
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {

    $block = parent::render();

    $node = node_load($nid);

    // hide fields we don't want to show

    $output = node_view($node, 'teaser', FALSE, FALSE);  //@todo : make this configurable
    $block['content'] = $output;

    return $block;
  }

}