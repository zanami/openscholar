/**
 * jQuery behaviors for site welcome tour.
 */
(function ($) {
  Drupal.behaviors.os_notifications = {
    attach: function (context, settings) {

      // Only continues if we have the hopscotch library defined.
      if (typeof hopscotch == 'undefined') {
        return;
      }

      // Only continues if we have the link in the DOM.
      var tourLink = '#os-tour-site-welcome';
      if (!$(tourLink).length) {
        return;
      }

      // Sets up the tour object with the loaded feed item steps.
      var tour = {
        showPrevButton: true,
        scrollTopMargin: 100,
        id: "os-tour-site-welcome",
        steps: [
          {
            title: 'Welcome!',
            content: 'In this short tour, you\'ll learn all about layout and widgets.',
            target: document.querySelector('#content'),
            placement: "left"
          }
        ]
      };

      // Adds our tour overlay behavior with desired effects
      $(tourLink, context).once('osTourSiteWelcome', function() {
        $(this).click(function() {
          $('html, body').animate({scrollTop:0}, '500', 'swing', function() {
            $('.hopscotch-bubble').addClass('animated');
            hopscotch.startTour(tour);
            // Removes animation for each step.
            $('.hopscotch-bubble').removeClass('animated');
            // Allows us to target just this tour in CSS rules.
            $('.hopscotch-bubble').addClass('os-tour-site-welcome');
          });
        });
      });
    }
  };

})(jQuery);
