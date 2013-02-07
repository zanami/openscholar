/**
 * @file os_slideshow_flexslider.js
 * 
 * Initializes flexslider javascript
 */

(function ($) {

// Behavior to load FlexSlider
Drupal.behaviors.os_slideshow = {
  attach: function(context, settings) {
    $(window).load(function() {
      //$('.flexslider').flexslider(Drupal.settings.os_slideshow.flexslider); //there  should be an of sliders?  then we can loda them by class and ssetting.
      for (delta in Drupal.settings.os_slideshow) {
        var div = 'div#boxes-box-' + delta;
        $(div + ' .flexslider').flexslider(Drupal.settings.os_slideshow[delta])
      }
    });
  }
};

}(jQuery));
