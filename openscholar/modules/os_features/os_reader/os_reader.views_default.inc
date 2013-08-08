<?php
/**
 * @file
 * os_reader.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function os_reader_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'os_reader';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'os_feed_item';
  $view->human_name = 'OS Reader';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'OS Reader';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: OS feed item: Label */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'os_feed_item';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: OS feed item: Description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'os_feed_item';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = '';
  $handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
  /* Field: OS feed item: Copy Feed to News */
  $handler->display->display_options['fields']['copy_feed_to_news']['id'] = 'copy_feed_to_news';
  $handler->display->display_options['fields']['copy_feed_to_news']['table'] = 'os_feed_item';
  $handler->display->display_options['fields']['copy_feed_to_news']['field'] = 'copy_feed_to_news';
  $handler->display->display_options['fields']['copy_feed_to_news']['label'] = '';
  $handler->display->display_options['fields']['copy_feed_to_news']['element_label_colon'] = FALSE;
  /* Sort criterion: Broken/missing handler */
  $handler->display->display_options['sorts']['time']['id'] = 'time';
  $handler->display->display_options['sorts']['time']['table'] = 'os_feed_item';
  $handler->display->display_options['sorts']['time']['field'] = 'time';
  $handler->display->display_options['sorts']['time']['order'] = 'DESC';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'os-reader';
  $export['os_reader'] = $view;

  return $export;
}
