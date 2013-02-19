/**
 * @file os_slideshow_flexslider.js
 * 
 * Initializes flexslider javascript, attaches back, pause, next, pager functions
 */

(function ($) {

// Behavior to load FlexSlider
Drupal.behaviors.os_slideshow = {
  attach: function(context, settings) {
//    function slider_closure(selector) {
//      return function() {
//        return $(selector);
//      }
//    }
    
    //event to mark active page in pager
//    function markpage(e) {
//      var slider = e.data().flexslider 
//      var page = slider.currentSlide      
//      $(slider).find('.flex-page-link').removeClass('active')
//      $(slider).find('#flex-page-'+page).addClass('active')
//    }
    
    $(window).load(function() {
      for (delta in Drupal.settings.os_slideshow) {
        //start slideshow
        var div = 'div#' + delta;
        var slider = $(div + ' .rslides');        
        //Drupal.settings.os_slideshow[delta]['after'] = markpage;
        slider.responsiveSlides(Drupal.settings.os_slideshow[delta]);
      }
    });
  }
}


}(jQuery));
