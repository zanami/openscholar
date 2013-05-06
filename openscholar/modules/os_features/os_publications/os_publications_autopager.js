/**
 * @file os_publications_autopager.js
 * 
 * Initializes autopager library for infinite scrolling.
 * http://lagoscript.org/jquery/autopager
 */

(function($) {
  
  Drupal.behaviors.os_publications = {
    attach: function (context, settings) {
      if (typeof(jQuery.autopager) == 'function') {
        for (var mod in Drupal.settings.autopager) {
          var $pager = $.autopager(Drupal.settings.autopager[mod]);
        }
        //hide the pager?
      }
    }
      
  }
  
})(jQuery);