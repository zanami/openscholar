<?php

class os_boxes_random_post extends os_boxes_manual_list {

  function render() {
    $title = isset($this->title) ? $this->title : NULL;
    $block['title'] = $title;
    $block['delta'] = $this->delta;
    $block['content'] = '';

    $k = array_rand($this->options['nodes']);
    if ($node = node_load($this->options['nodes'][$k]['nid'])) {
      if ($this->options['style'] == 'teasers') {
        // Theme the nodes as a teaser

        //Node may not load if it has since been deleted
        $block['content'] = render(node_view($node, 'teaser'));
      }
      elseif ($this->options['style'] == 'full') {
        $block['content'] = render(node_view($node, 'full'));
      }
      else {
        $l = array(
          'href' => 'node/'.$node->nid,
          'title' => $node->title,
        );
        $options = array(
          'absolute' => true,
        );
        $block['content'] = l($l['title'], $l['href'], $options);
      }
    }

    return $block;
  }
}