<?php

/* boxes apis add/remove etc */

function os_boxes_box_save($delta, $box) {
  if (module_exists('spaces') && $space = spaces_get_space()) {
    $space->controllers->boxes->set($box->delta, $box);
  }  
}

function os_boxes_box_load($delta) {
  if (module_exists('spaces') && $space = spaces_get_space()) {
    return $space->controllers->boxes->get($delta);
  } else {
    return boxes_box_load($delta);
  }
}


if (module_exists('spaces') && $space = spaces_get_space()) {
  if ($box = $space->controllers->boxes->get($node->widget)) {
    $box->options['nodes'][(int)$node->nid] = array(
        'nid' => (int)$node->nid,
        'weight' => 1 + array_reduce($box->options['nodes'], '_heaviest', 0),
    );
    $space->controllers->boxes->set($box->delta, $box);
  }