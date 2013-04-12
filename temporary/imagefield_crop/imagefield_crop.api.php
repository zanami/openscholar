<?php

/**
 * This hook is use to notify other module about changed crop area.
 * @param $file
 * @return void
 */
function hook_imagefield_crop_image_crop_updated($file) {

}

/**
 * This hook is called after the field instance is loaded.  Altering the
 * instance allows you to change the resolution to crop to, without having
 * to create another field.  Context provides the form the field appears in.
 */
function hook_imagefield_crop_instance_alter(&$instance, $context) {

}
