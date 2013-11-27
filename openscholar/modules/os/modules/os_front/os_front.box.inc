<?php
/**
 * @file
 * iqss_scholars_front.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_front_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_front_header';
  $box->plugin_key = 'os_boxes_html';
  $box->title = '';
  $box->description = 'Front Page Header Text';
  $box->options = array(
    'text' => array(
      'value' => '<img src="" class="os-title-image" alt="OpenScholar"> <strong>Welcome!</strong> Lorem ipsum dolor sit amet, consectetuer
          adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat.',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['os_front_header'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_front_create';
  $box->plugin_key = 'os_boxes_html';
  $box->title = '';
  $box->description = 'Create A Site';
  $box->options = array(
    'text' => array(
      'value' => 'Create a Site',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['os_front_create'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_front_main_support';
  $box->plugin_key = 'os_boxes_html';
  $box->title = 'Support';
  $box->description = 'Support';
  $box->options = array(
    'text' => array(
      'value' => 'We have a very active community-style support site where questions, ideas, and problems can be searched for, posted, and replied to. <a href="">Visit the Community</a>',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['os_front_main_support'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_front_main_configure';
  $box->plugin_key = 'os_boxes_html';
  $box->title = '';
  $box->description = 'Create A Site';
  $box->options = array(
    'text' => array(
      'value' => 'Want to change the design/theme of this page or change the content that appears? <a href="">Find out how.</a>',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['os_front_main_configure'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_front_sample';
  $box->plugin_key = 'os_boxes_html';
  $box->title = '';
  $box->description = 'Create A Site';
  $box->options = array(
    'text' => array(
      'value' => 'Same Websites <ul>
        <li>
          <a src=""><img src=""></a>
        </li>
        <li>
          <a src=""><img src=""></a>
        </li>
        <li>
          <a src=""><img src=""></a>
        </li>
      </ul>',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['os_front_sample'] = $box;

  return $export;
}
