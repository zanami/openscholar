<?php
/**
 * @file
 * os_software.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function os_software_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'os_software_projects';
  $view->description = 'Os Software > Software';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'OS Software Projects';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Software';
  $handler->display->display_options['access']['type'] = 'spaces_feature';
  $handler->display->display_options['access']['spaces_feature'] = '0';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty text';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'There are currently no projects available.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: Content: Sticky */
  $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['table'] = 'node';
  $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'software_project' => 'software_project',
  );
  /* Filter criterion: Spaces: Content in current space */
  $handler->display->display_options['filters']['current']['id'] = 'current';
  $handler->display->display_options['filters']['current']['table'] = 'spaces';
  $handler->display->display_options['filters']['current']['field'] = 'current';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'software';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Software';
  $handler->display->display_options['menu']['name'] = 'primary-menu';

  /* Display: Entity Reference */
  $handler = $view->new_display('entityreference', 'Entity Reference', 'entityreference_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'entityreference_style';
  $handler->display->display_options['style_options']['search_fields'] = array(
    'title' => 'title',
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entityreference_fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $export['os_software_projects'] = $view;

  $view = new view();
  $view->name = 'os_software_releases';
  $view->description = 'Os Software > Recommended Releases, All Releases, Recent Releases, All Recent Releases';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'OS Software Releases';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'All Releases';
  $handler->display->display_options['access']['type'] = 'spaces_feature';
  $handler->display->display_options['access']['spaces_feature'] = 'os_software';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '15';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Content: Package (field_software_package:fid) */
  $handler->display->display_options['relationships']['field_software_package_fid']['id'] = 'field_software_package_fid';
  $handler->display->display_options['relationships']['field_software_package_fid']['table'] = 'field_data_field_software_package';
  $handler->display->display_options['relationships']['field_software_package_fid']['field'] = 'field_software_package_fid';
  $handler->display->display_options['relationships']['field_software_package_fid']['label'] = 'Package';
  $handler->display->display_options['relationships']['field_software_package_fid']['required'] = TRUE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_software_project_target_id']['id'] = 'field_software_project_target_id';
  $handler->display->display_options['relationships']['field_software_project_target_id']['table'] = 'field_data_field_software_project';
  $handler->display->display_options['relationships']['field_software_project_target_id']['field'] = 'field_software_project_target_id';
  $handler->display->display_options['relationships']['field_software_project_target_id']['label'] = 'Project';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: File: Size */
  $handler->display->display_options['fields']['filesize']['id'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filesize']['field'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['relationship'] = 'field_software_package_fid';
  $handler->display->display_options['fields']['filesize']['exclude'] = TRUE;
  /* Field: Content: Package */
  $handler->display->display_options['fields']['field_software_package']['id'] = 'field_software_package';
  $handler->display->display_options['fields']['field_software_package']['table'] = 'field_data_field_software_package';
  $handler->display->display_options['fields']['field_software_package']['field'] = 'field_software_package';
  $handler->display->display_options['fields']['field_software_package']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_software_package']['alter']['text'] = '<a href="[field_software_package]">Download</a> ([filesize])';
  $handler->display->display_options['fields']['field_software_package']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_software_package']['type'] = 'file_url_plain';
  /* Field: Content: Link URL */
  $handler->display->display_options['fields']['field_links_link']['id'] = 'field_links_link';
  $handler->display->display_options['fields']['field_links_link']['table'] = 'field_data_field_links_link';
  $handler->display->display_options['fields']['field_links_link']['field'] = 'field_links_link';
  $handler->display->display_options['fields']['field_links_link']['label'] = 'Release info';
  $handler->display->display_options['fields']['field_links_link']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_links_link']['type'] = 'link_label';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date';
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'M j Y';
  /* Sort criterion: Content: Sticky */
  $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['table'] = 'node';
  $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  /* Contextual filter: Content: Project (field_software_project) */
  $handler->display->display_options['arguments']['field_software_project_target_id']['id'] = 'field_software_project_target_id';
  $handler->display->display_options['arguments']['field_software_project_target_id']['table'] = 'field_data_field_software_project';
  $handler->display->display_options['arguments']['field_software_project_target_id']['field'] = 'field_software_project_target_id';
  $handler->display->display_options['arguments']['field_software_project_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_software_project_target_id']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['field_software_project_target_id']['title'] = '%1';
  $handler->display->display_options['arguments']['field_software_project_target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['field_software_project_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_software_project_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_software_project_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_software_project_target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_software_project_target_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['field_software_project_target_id']['validate_options']['types'] = array(
    'software_project' => 'software_project',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'software_release' => 'software_release',
  );
  /* Filter criterion: Spaces: Content in current space */
  $handler->display->display_options['filters']['current']['id'] = 'current';
  $handler->display->display_options['filters']['current']['table'] = 'spaces';
  $handler->display->display_options['filters']['current']['field'] = 'current';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'node';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'field_software_project_target_id';
  $handler->display->display_options['filters']['status_1']['value'] = '1';

  /* Display: All Releases Page */
  $handler = $view->new_display('page', 'All Releases Page', 'page');
  $handler->display->display_options['path'] = 'node/%/releases';

  /* Display: Recent Releases */
  $handler = $view->new_display('block', 'Recent Releases', 'block');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Recent Releases';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';

  /* Display: All Recent Releases */
  $handler = $view->new_display('block', 'All Recent Releases', 'block_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'All Recent Releases';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '3';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'filesize' => 'filesize',
    'field_links_link' => 'field_links_link',
    'created' => 'created',
  );
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'software_release' => 'software_release',
  );
  /* Filter criterion: Spaces: Content in current space */
  $handler->display->display_options['filters']['current']['id'] = 'current';
  $handler->display->display_options['filters']['current']['table'] = 'spaces';
  $handler->display->display_options['filters']['current']['field'] = 'current';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'node';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'field_software_project_target_id';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  /* Filter criterion: Content: Recommended (field_software_recommended) */
  $handler->display->display_options['filters']['field_software_recommended_value']['id'] = 'field_software_recommended_value';
  $handler->display->display_options['filters']['field_software_recommended_value']['table'] = 'field_data_field_software_recommended';
  $handler->display->display_options['filters']['field_software_recommended_value']['field'] = 'field_software_recommended_value';
  $handler->display->display_options['filters']['field_software_recommended_value']['value'] = array(
    1 => '1',
  );

  /* Display: Recommended Releases */
  $handler = $view->new_display('block', 'Recommended Releases', 'block_2');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'software_release' => 'software_release',
  );
  /* Filter criterion: Spaces: Content in current space */
  $handler->display->display_options['filters']['current']['id'] = 'current';
  $handler->display->display_options['filters']['current']['table'] = 'spaces';
  $handler->display->display_options['filters']['current']['field'] = 'current';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'node';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'field_software_project_target_id';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  /* Filter criterion: Content: Recommended (field_software_recommended) */
  $handler->display->display_options['filters']['field_software_recommended_value']['id'] = 'field_software_recommended_value';
  $handler->display->display_options['filters']['field_software_recommended_value']['table'] = 'field_data_field_software_recommended';
  $handler->display->display_options['filters']['field_software_recommended_value']['field'] = 'field_software_recommended_value';
  $handler->display->display_options['filters']['field_software_recommended_value']['value'] = array(
    1 => '1',
  );

  /* Display: Recommended */
  $handler = $view->new_display('entity_view', 'Recommended', 'entity_view_1');
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'software_project',
  );
  $export['os_software_releases'] = $view;

  return $export;
}
