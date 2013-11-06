<?php
/**
 * @file
 * os_events.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function os_events_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "feeds_tamper" && $api == "feeds_tamper_default") {
    return array("version" => "2");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function os_events_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_default_message_type().
 */
function os_events_default_message_type() {
  $items = array();
  $items['os_event_registration'] = entity_import('message_type', '{
    "name" : "os_event_registration",
    "description" : "OS event registration",
    "argument_keys" : [],
    "argument" : [],
    "category" : "message_type",
    "data" : {
      "token options" : { "clear" : 0 },
      "purge" : { "override" : 0, "enabled" : 0, "quota" : "", "days" : "" }
    },
    "language" : "",
    "arguments" : null,
    "message_text" : { "und" : [
        {
          "value" : "\\u003Cp\\u003E[message:field-node-reference:title] event registration confirmation\\u003Ca title=\\u0022Insert this token into your form\\u0022\\u003E\\u003Cbr\\u003E\\u003C\\/a\\u003E\\u003C\\/p\\u003E",
          "format" : "filtered_html",
          "safe_value" : "\\u003Cp\\u003E[message:field-node-reference:title] event registration confirmation\\u003Ca title=\\u0022Insert this token into your form\\u0022\\u003E\\u003Cbr \\/\\u003E\\u003C\\/a\\u003E\\u003C\\/p\\u003E"
        },
        {
          "value" : "\\u003Cp\\u003EHello [message:field-registration-reference:field_full_name].\\u003Cbr\\u003E\\u003Cbr\\u003EYou registered to the event \\u003Ca href=\\u0022[message:field-node-reference:url]\\u0022 data-url=\\u0022[message:field-node-reference:url]\\u0022\\u003E[message:field-node-reference:title]\\u003C\\/a\\u003E\\u0026nbsp;successfully.\\u003C\\/p\\u003E",
          "format" : "full_html",
          "safe_value" : "\\u003Cp\\u003EHello [message:field-registration-reference:field_full_name].\\u003Cbr \\/\\u003E\\u003Cbr \\/\\u003EYou registered to the event \\u003Ca href=\\u0022[message:field-node-reference:url]\\u0022 data-url=\\u0022[message:field-node-reference:url]\\u0022\\u003E[message:field-node-reference:title]\\u003C\\/a\\u003E\\u00a0successfully.\\u003C\\/p\\u003E\\n"
        }
      ]
    },
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_node_info().
 */
function os_events_node_info() {
  $items = array(
    'event' => array(
      'name' => t('Event'),
      'base' => 'node_content',
      'description' => t('A calendar event with start and optional end times.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
