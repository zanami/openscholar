<?php

class os_boxes_accordion extends os_boxes_widget_collection {


  function render() {
    $output = parent::render();

    $content = array(
      '#prefix' => '<div class="accordion">',
      '#suffix' => '</div>',
      '#attached' => array(
        'library' => array(
          array('system', 'ui.accordion'),
        ),
        'js' => array(
          drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_accordion/os_boxes_accordion.render.js',
        )
      )
    );

    module_load_include('module', 'block', 'block');
    $plugin = context_get_plugin('reaction', 'block');
    $blocks = $plugin->get_blocks();

    foreach ($this->options['widgets'] as $k => $w) {
      if ($b = $blocks[$w['bid']]) {
        $b->region = $this->delta;
        if ($b_out = _block_render_blocks(array($b))) {
          if (!empty($b_out[$b->module.'_'.$b->delta]->content)) {

            $block_output = _block_get_renderable_array($b_out);
            unset($block_output[$b->module.'_'.$b->delta]['#theme_wrappers']);
            $content[$k] = array(
              '#weight' => $w['weight'],
              'title' => array(
                '#prefix' => '<h3>',
                '#suffix' => '</h3>',
                '#markup' => $w['title'],
              ),
              'body' => array(
                '#prefix' => '<div class="accordion-panel">',
                '#suffix' => '</div>',
                'output' => $block_output,
              )
            );
          }
        }
      }
    }

    $output['content'] = drupal_render($content);
    return $output;
  }
}