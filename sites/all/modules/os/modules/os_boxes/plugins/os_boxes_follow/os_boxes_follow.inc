<?php

class os_boxes_follow extends os_boxes_default {

  public function options_defaults() {
    return array(
      'links' => array(),
    );
  }

  public function options_form() {
    $form = array();
  	vsite_include('nodeautocomplete');
    $form['#after_build'] = array('os_boxes_follow_add_externals');

    $form['add'] = array(
      '#type' => 'fieldset',
      '#title' => t('Add New Post'),
      'button' => array(
        '#type' => 'markup',
        '#value' => '<input type="button" class="add_new" value="Add" />',
      ),
    );

    // build a normal form
    $form['links'] = array(
      '#type' => 'markup',
      '#tree' => TRUE,
      '#pre_render' => array('os_boxes_follow_table'),
    );

    $links = &$form['links'];
    $this->options['links']['blank'] = array('title' => '__blank__', 'weight' => 2000000); // blank element for copying purposes
    foreach ($this->options['links'] as $k => $v) {
      $n = array(
        '#weight' => $v['weight'],
        'text' => array(
          '#type' => 'markup',
          '#value' => '<span>'.$v['title'].'</span> <a href="#" class="remove" title="Remove Link">Remove</a>',
        ),
        'title' => array(
          '#type' => 'hidden',
          '#default_value' => $v['title'],
        ),
        'weight' => array(
          '#type' => 'textfield',
          '#default_value' => $v['weight'],
          '#size' => 3,
          '#attributes' => array(
            'class' => (string)$k!='blank'?'field-weight':'',
          )
        ),
      );
      $nodes[$k] = $n;
    }
    uasort($this->options['links'], 'element_sort');

    $form['count'] = array(
      '#type' => 'hidden',
      '#default_value' => count($this->options['links'])-1,
      '#process' => array('os_boxes_follow_add_new_fields'),
    );

    return $form;
  }

  public function render() {
    $block = parent::render();
    $links = array();
    foreach ($this->options['links'] as $n) {
      $l = array(
        'href' => 'node/'.$n['nid'],
        'title' => $n['title'],
      );
      $links[$n['weight']] = $l;
    }
    ksort($links);
    $block['content'] = theme('links', $links);
    return $block;
  }
}

function os_boxes_follow_add_externals($form) {
  drupal_add_js(drupal_get_path('module', 'os_boxes').'/theme/js/os_boxes_follow.js');
  return $form;
}

function os_boxes_follow_table($element) {
  // render the form as a table
  $rows = array();
  uasort($element, 'element_sort');
  foreach (element_children($element) as $c) {
    $rows[] = array(
      'data' => array(
        drupal_render($element[$c]['nid']).drupal_render($element[$c]['text']).drupal_render($element[$c]['title']),
        drupal_render($element[$c]['weight']),
      ),
      'class' => (string)$c!='blank'?'draggable':'',
    );
  }
  $element['#value'] = theme('table', array(), $rows, array('id' => 'follow-links-list'));
  drupal_add_tabledrag('follow-links-list', 'order', 'sibling', 'field-weight');
  return $element;
}

function os_boxes_follow_add_new_fields(&$element, $post, &$form_state, &$form) {
  for ($i=$element['#default_value']; $i < $post['count']; $i++) {
    $form['options']['links'][$i] = $form['options']['links']['blank'];
    $form['options']['links'][$i] += array(
      '#attributes' => array('id' => $i),
      '#parents' => array('nodes', $i),
      '#array_parents' => array('options', 'links', $i),
      '#tree' => TRUE,
      '#post' => $element['#post'],
    );
    form_builder($form['form_id']['#value'], $form['options']['links'][$i], $form_state);
  }
  unset($form_state['values']['links']['blank']);

  // remove nids that aren't in the post
  foreach ($form_state['values']['nodes'] as $k => $v) {
    if (!isset($form_state['input']['nodes'][$k])) {
      unset($form_state['values']['nodes'][$k]);
    }
  }

  return $element;
}