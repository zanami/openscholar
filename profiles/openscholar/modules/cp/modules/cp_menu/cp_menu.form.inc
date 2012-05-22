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
      '#weight' => (($is_active && $menu == $m)?1:$weight++),
      '#hidden' => ($is_active && $menu != $m),
      'menu-name' => array(
        '#type' => 'hidden',
        '#value' => $m,
        '#attributes' => array(
          'class' => array(
            'menu-name',
          ),
        ),
      ),
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
              'menu-name',
              'menu-name-'. $l['menu_name']
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
          '#default_value' => $l['plid'],
          '#attributes' => array(
            'class' => array(
              'menu-plid',
              'menu-plid-'.$l['menu_name']
            ),
          ),
        ),
        'mlid' => array(
          '#type' => 'hidden',
          '#default_value' => $l['mlid'],
          '#attributes' => array(
            'class' => array(
              'menu-mlid',
              'menu-mlid'.$l['menu_name'],
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
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'cp_menu').'/cp_menu.form.js',
      ),
    ),
  );

  $form['#submit'] = array(
    'cp_menu_submit_form',
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
    $extra_classes = array();
    if ($table_data[$d]['#hidden']) {
      $extra_classes[] = 'hidden';
    }
    $rows[] = array(
      'data' => array(
        array(
          'data' => $menus[$d].drupal_render($table_data[$d]['menu-name']),
          'colspan' => 4,
        )
      ),
      'class' => array_merge(array('section-heading'), $extra_classes),
    );
    $has_links = false;
    foreach (element_children($table_data[$d]) as $link_name) {
      if ($link_name == 'menu-name') continue;
      $l = $table_data[$d][$link_name];
      $has_links = true;
      $rows[] = array(
        'data' => array(
          theme('indentation', array('size' => $l['#depth'] - 1)).drupal_render($l['title']),
          drupal_render($l['edit_link']),
          drupal_render($l['menu']),
          drupal_render($l['weight']).drupal_render($l['plid']).drupal_render($l['mlid']),
        ),
        'class' => array_merge(array('draggable'), $extra_classes),
      );
    }
    $rows[] = array(
      'data' => array(
        array(
       	  'data' => 'This menu has no links.',
       	  'colspan' => 4,
        ),
      ),
      'class' => array('section-message', $has_links?'section-populated':'section-empty'),
    );
  }
  //drupal_add_tabledrag('edit-menu-table', 'match', 'sibling', 'menu-name', 'menu-name', 'menu-name', FALSE);
  drupal_add_tabledrag('edit-menu-table', 'match', 'parent', 'menu-plid', 'menu-plid', 'menu-mlid', TRUE, MENU_MAX_DEPTH - 1);
  drupal_add_tabledrag('edit-menu-table', 'order', 'sibling', 'menu-weight');
  return theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 'edit-menu-table')));
}

function cp_menu_submit_form($form, &$form_state) {
  foreach ($form_state['values']['menus'] as $links) {
    foreach ($links as $l) {
      if (!is_array($l)) continue;

      $link = os_menu_link_load($l['menu'], $l['mlid']);
      $link['weight'] = $l['weight'];
      $link['menu_name'] = $l['menu'];
      $link['plid'] = $l['plid'];
      os_menu_link_save($link);
    }
  }
}