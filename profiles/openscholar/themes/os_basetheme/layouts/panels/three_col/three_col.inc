<?php
function adaptivetheme_three_col_panels_layouts() {
  $items['three_col'] = array(
    'title'    => t('OS Three column 33/34/33'),
    'category' => t('OS Responsive Panels - 3 column'),
    'icon'     => 'three_col.png',
    'theme'    => 'three_col',
    'admin css' => 'three_col.admin.css',
    'theme arguments' => array('id', 'content'),
    'regions' => array(
      'three_col_top'    => t('Top (conditional)'),
      'three_col_first'  => t('Left'),
      'three_col_second' => t('Center'),
      'three_33_third'  => t('Right'),
      'three_33_bottom' => t('Bottom (conditional)'),
    ),
    // AT
    'type' => 'three',
    'options' => array(
      'three-col' => 'default',
      'three-col-stack-top' => 'stack bottom',
      'three-col-stack-bottom' => 'stack top',
      'three-col-stack' => 'stack',
    ),
    'styles' => array(
      'three-col' => array(
        'css' => array(
          '33' => array('.three-col .region' => 'width:33.333333%'),
        ),
      ),
       'three-col-stack-top' => array(
        'css' => array(
          '50' => array('.three-col .region' => 'width:50%'),
          'fdw' => array('.three-col .region-three-col-first' => 'float:none;display:block;width:100%;clear:both'),
        ),
      ),
      'three-col-stack-bottom' => array(
        'css' => array(
          '50' => array('.three-col .region' => 'width:50%'),
          'fdw' => array('.three-col .region-three-col-third' => 'float:none;display:block;width:100%;clear:both'),
        ),
      ),
      'three-col-stack' => array(
        'css' => array(
          'fdw' => array('.three-col .region' => 'float:none;display:block;width:100%;clear:both'),
        ),
      ),
    ),
  );

  return $items;
}

function template_preprocess_three_col(&$vars) {
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
