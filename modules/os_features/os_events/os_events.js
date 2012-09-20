/**
 * 
 */
Drupal.behaviors.os_events = {
  attach: function (ctx) {
    jQuery(document).bind('drupalOverlayOpen', function () {
      jQuery.colorbox.close();
    });
  }
};