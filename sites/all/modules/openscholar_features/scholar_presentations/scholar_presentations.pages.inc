<?php

function scholar_presentations_settings() {
  $form = array();
  if (module_exists('slideshareapi')) {
    $form = slideshareapi_settings();
  }

  unset($form['slideshareapi']['slideshareapi_apiurl']);
  $vsite = vsite_get_vsite();
  $form['slideshareapi']['slideshareapi_key']['#default_value'] = $vsite->controllers->variable->get('slideshareapi_key', 'space');
  $form['slideshareapi']['slideshareapi_secret']['#default_value'] = $vsite->controllers->variable->get('slideshareapi_secret', 'space');
  $form['slideshareapi']['slideshareapi_user']['#default_value'] = $vsite->controllers->variable->get('slideshareapi_user', 'space');
  $form['slideshareapi']['slideshareapi_pass']['#default_value'] = $vsite->controllers->variable->get('slideshareapi_pass', 'space');

  // detect for presentations that haven't been uploaded


  // system settings form is run by slideshareapi_settings
  // don't need to do it again here
  return $form;
}