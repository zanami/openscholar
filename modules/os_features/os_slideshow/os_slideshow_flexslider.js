/**
 * @file os_slideshow_flexslider.js
 * 
 * Initializes flexslider javascript, attaches back, pause, next, pager functions
 */

(function ($) {

// Behavior to load FlexSlider
Drupal.behaviors.os_slideshow = {
  attach: function(context, settings) {
    $(window).load(function() {
      for (delta in Drupal.settings.os_slideshow) {
        //start slideshow
        var div = 'div#boxes-box-' + delta;
        var slider = $(div + ' .flexslider');
        slider.flexslider(Drupal.settings.os_slideshow[delta])

        //bind controls 
        slider.find('#flex-next').click(function() {slider.flexslider('next');});
        slider.find('#flex-prev').click(function() {slider.flexslider('prev');});
        slider.find('#flex-pause').click(function() {
          if (slider.hasClass('pause')) {
            slider.removeClass('pause').flexslider('play');
          } else {
            slider.addClass('pause').flexslider('pause');
          }
        });

        //closure for jump to page functions
        function pager_closure(page) {
          return function() {
            slider.flexslider(page);
          }
        }

        //bind pager functions
        for (var i=0; i<slider.find('img').length; i++) {
          var fn = pager_closure(i);
          slider.find('#flex-page-'+i).click(fn);
        }
      }
    });
  }
};

}(jQuery));
