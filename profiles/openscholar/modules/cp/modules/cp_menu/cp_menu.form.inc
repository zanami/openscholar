<?php

/**
 * Builds a form for configuring a menu or all of them
 *
 * @param string $menu - which menu we're going to operate on by default
 * Other menus will be hidden to start and displayed when the user opts to
 * move a link to another menu
 */
function cp_menu_build_form($form, $form_state, $menu = '') {
  ctools_include('ajax');
  ctools_include('modal');
  ctools_include('menu', 'os');

  $menus = os_get_menus();
  $menus += array(
    'no-menu' => 'Not In Menu',
  );
  $is_active = false;
  if (isset($menus[$menu])) {
    $is_active = true;
  }
  $weight = 5;

  $form['menus'] = array(
    '#theme' => 'cp_menu_table',
    '#tree' => true,
  );

  foreach ($menus as $m => $title) {
    $form['menus'][$m] = array(
      '#weight' => ($is_active?1:$weight++),
    );
    $links = _cp_menu_flatten_tree(os_menu_tree_data($m));
    foreach ($links as $link) {
      $l = $link['link'];
      $url = url($l['link_path'], array('absolute' => true));
      $form['menus'][$m][$l['mlid']] = array(
        'title' => array(
          '#markup' => $l['link_title'],
          '#description' => l($url, $url),
        ),
        'edit_link' => array(
          '#markup' => ctools_modal_text_button('Edit Link', 'cp/build/menu/nojs/', ''),
        ),
        'menu' => array(
          '#type' => 'select',
          '#options' => $menus,
          '#default_value' => $l['menu_name'],
          '#attributes' => array(
            'class' => array(
              'menu-select',
              'menu-select-'. $l['menu_name']
            ),
          ),
        ),
        'weight' => array(
          '#type' => 'textfield',
          '#default_value' => $l['weight'],
          '#size' => 3,
          '#attributes' => array(
            'class' => array(
              'menu-weight',
              'menu-weight-'. $l['menu_name']
            ),
          ),
        ),
        'plid' => array(
          '#type' => 'hidden',
          '#default_valuee' => $l['plid'],
          '#attributes' => array(
            'class' => array(
              'menu-plid',
              'menu-plid-'.$l['menu_name']
            ),
          ),
        ),
        'menu-old' => array(
          '#type' => 'hidden',
          '#value' => $l['menu_name'],
        ),
        '#depth' => $l['depth'],
      );
    }
  }

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save settings'),
    ),
  );

  return $form;
}

function _cp_menu_flatten_tree($tree) {
  $copy = array();
  foreach ($tree as $l) {
    $copy = array_merge($copy, array($l), _cp_menu_flatten_tree($l['below']));
  }

  return $copy;
}

/**
 * Renders the form array as a table and sets up the tabledrag
 * @param unknown_type $variables
 */
function theme_cp_menu_table($variables) {
  $table_data = $variables['table'];
  $menus = os_get_menus();
  $menus += array(
    'no-menu' => 'Not In Menu',
  );

  $headers = array(
    t('Title'),
    t('Edit'),
    t('Menu'),
    t('Weight'),
  );
  uasort($table_data, 'element_sort');

  $rows = array();
  foreach (element_children($table_data) as $d) {
    $rows[] = array(
      'data' => array(
        array(
          'data' => $menus[$d],
          'colspan' => 4,
        ),
      )
    );
    foreach (element_children($table_data[$d]) as $link_name) {
      $l = $table_data[$d][$link_name];
      $rows[] = array(
        'data' => array(
          theme('indentation', array('size' => $l['#depth'] - 1)).drupal_render($l['title']),
          drupal_render($l['edit_link']),
          drupal_render($l['menu']),
          drupal_render($l['weight']).drupal_render($l['plid']),
        ),
        'class' => array('draggable'),
      );
    }
  }
  drupal_add_tabledrag('edit-menu-table', 'match', 'parent', 'menu-plid', 'menu-plid', 'menu-mlid', TRUE, MENU_MAX_DEPTH - 1);
  drupal_add_tabledrag('edit-menu-table', 'order', 'sibling', 'menu-weight');
  return theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 'edit-menu-table')));
}