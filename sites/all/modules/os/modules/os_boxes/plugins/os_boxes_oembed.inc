<?php

/**
 * @file
 * The class for "Embed" type widgets.
 */

class os_boxes_oembed extends os_boxes_default {
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();

    $options['selection'] = 'oembed_url';
    $options['oembed_url'] = '';
    $options['pasted'] = '';
    $options['width'] = 200;

    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){

    ctools_include('dependent');
    ctools_add_js('dependent');
    $form['selection'] = array(
      '#title' => t('Use'),
      '#type' => 'radios',
      '#options' => array(
        'oembed_url' => t('Embed.ly Provider URL'),
        'pasted' => t('Original embed code.')
      ),
      '#default_value' => $this->options['selection'],
      '#description' => t('Which source do you want to use?'),
      '#tree' => TRUE,
    );

    $form['oembed_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Embed.ly Provider URL'),
      '#default_value' => $this->options['oembed_url'],
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('radio:selection' => array('oembed_url')),
      '#description' => t('Link to embed.ly supported provider (ie YouTube, Flickr, imgur).'),
    );

    $form['pasted'] = array(
      '#type' => 'textarea',
      '#title' => t('HTML embed tag'),
      '#default_value' => $this->options['pasted'],
      '#description' => t('Use html embed tag to provide swf or other embeddable media.'),
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('radio:selection' => array('pasted'))
    );

    $form['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum Width of Embed'),
      '#default_value' => $this->options['width'],
      '#description' => t('The maximum width of the embedded resource, in pixels.'),
    );

    $form += parent::options_form();
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    //@todo
    $block = parent::render();
    switch ($this->options['selection']) {
      case 'oembed_url':
        $block['content'] = oembed_filter('process', 0, -1, $this->options['oembed_url']);
      break;
      case 'pasted':
        $block['content'] = embedfilter_filter('process', 0, -1, $this->options['pasted']);
      break;
    }
    $block['content'] = embedfilter_media_resize($block['content'], $this->options['width']);
    return $block;
  }

}