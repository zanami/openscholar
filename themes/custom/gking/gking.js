/**
 * Custom jQuery effects for gking theme.
 */
(function ($) {
  Drupal.behaviors.gking = {
    attach: function (context) {

      /**
       * Prepares Bio overlay for front page header.
       */
      // Bio set to display: none by default, for browsers without js.
      var container = $(".front .region-header-second .block-boxes-os_boxes_bio");
      if (container.length) {
        container.toggle();
      
        // Adds necessary more/close links to show/hide bio node content.
        $('<a class="more" href="#">More</a>')
          .appendTo('.front .region-header-second .block-boxes-os_boxes_html .boxes-box-content');
        container.find('.node')
          .prepend('<a class="more" href="#">CLOSE X</a>');
        container.find('.node .node-content')
          .prepend('<h3 class="cv-direct-download">Full CV: <a href="http://gking.harvard.edu/vitae/vitae.pdf">PDF</a></h3><div class="clear"></div>');

        // Both the "More" and "Close X" links trigger this animate event.
        $(".front .region-header-second a.more").click(function (event) {
          if (container.hasClass("bio-open")) {
            container.removeClass("bio-open").animate({height: '0'}, "1500");
          } else {
            container.addClass("bio-open").animate({height: '70%'}, "1500");
          }
        });
      }
      
      /**
       * Prepares the "Areas of Research" front page taxonomy widget.
       */
      var $sel = '#content-column .block-boxes-os_taxonomy_fbt .boxes-box-content';
      var areas = $($sel + ' ul li ul li:not(.aor-processed)');
      
      // Stand back! Complex jQuery effects ahead!
      if (areas.length) {
    	  
    	// Updates the displayed taxonomy term item on hover event
        $($sel + ' ul li ul li:not(.aor-processed)').hover(function () {
          // Removes active class from previous item
          $($sel + ' ul li ul li.active')
            .removeClass('active');
          // Hides the previous item's description
          $($sel + ' ul li ul li div.description')
            .fadeOut('fast');
          // Hides the previous item's more link
          $($sel + ' .more')
            .hide();
          // Adds active class to new item, shows description with fadeIn.
          $(this)
            .addClass('active')
            .find('div.description')
            .fadeIn('fast');
        });
        
        // Initializes first hover event.
        $($sel + '.termchild-methods li:not(.aor-processed)')
          .filter(":first")
          .each(function (index) {
            $(this).hover();
        });
        
        // Marks all items as processed, so this only runs once per pageload.
        $($sel + ' ul li ul li')
          .addClass('aor-processed');
      }
      
      /**
       * Scrolls to the top of the page when you click the sort links
       */
//      if ($('body.page-taxonomy').length) {
//        $('a.term-admin-sort-link')
//          .click(function () {
//          window.scroll(0, 0);
//        });
//      }
    }
  };
})(jQuery);