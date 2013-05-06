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
          var settings = Drupal.settings.autopager[mod]
          
          //make functions of js strings for callbacks
          settings.load = Drupal.behaviors.os_publications.hide_redundant;
          
          console.log(settings);
          var $pager = $.autopager(settings);
        }
        //hide the pager?
      }
    },
    
    
    //only keep the first of each fo the selectors.  menat for biblio loading several 2010 pages
    hide_redundant: function() {
      selector = '.biblio-separator-bar';
      $selectors = $(selector);
        
      $selectors.each(function(selectors) {
        html = $(this).html()
        $selectors.filter(':contains("'+html+'"):not(:first)').remove() //filter(':contains("'+html+'")').remove();
      });
    }
      
  }
  
})(jQuery);