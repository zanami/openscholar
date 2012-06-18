<?php
function adaptivetheme_two_col_panels_layouts() {
  $items['two_col'] = array(
    'title'    => t('OS Two column 50/50'),
    'category' => t('OS Responsive Panels - 2 column'),
    'icon'     => 'two_col.png',
    'theme'    => 'two_col',
    'admin css' => 'two_col.admin.css',
    'theme arguments' => array('id', 'content'),
    'regions' => array(
      'two_col_top'    => t('Top (conditional)'),
      'two_col_first'  => t('Left'),
      'two_col_second' => t('Right'),
      'two_col_bottom' => t('Bottom (conditional)'),
    ),
    // AT
    'type' => 'two',
    'options' => array(
      'two-col' => 'default',
      'two-col-stack' => 'stack',
    ),
    'styles' => array(
      'two-col' => array(
        'css' => array(
          '50' => array('.two-col .region' => 'width:50%'),
        ),
      ),
      'two-col-stack' => array(
        'css' => array(
          'fdw' => array('.two-col .region' => 'float:none;display:block;width:100%;clear:both'),
        ),
      ),
    ),
  );

  return $items;
}

function template_preprocess_two_col(&$vars) {
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
