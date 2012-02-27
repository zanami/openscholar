<?php

function media_embed_settings() {
  $domains = variable_get('media_embed_whitelist', array());
  $form = array();

  $form['whitelist'] = array(
    '#type' => 'textarea',
    '#default_value' => implode("\n\r", $domains),
    '#description' => t('Enter the list of domains that users may add iframe, object, embed and script tags from. One domain per line.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
}

function media_embed_settings_submit($form, $form_state) {
  $domains = $form_state['values']['whitelist'];

  $domains = split("[\n|\r]", $domains);
  variable_set('media_embed_whitelist', $domains);
}