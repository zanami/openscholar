/**
 * 
 */
Drupal.behaviors.osColorbox = {
  attach: function (ctx) {
    jQuery(document).bind('drupalOverlayOpen', function () {
      if ('colorbox' in jQuery) {
        jQuery.colorbox.close();
      }
    });
  }
};