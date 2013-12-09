/**
 * jQuery behaviors for platform notification feeds.
 */
(function ($) {

  Drupal.behaviors.os_notifications = {

    attach: function (context, settings) {

      // Setup.
      var tourLink = '#os-tour-home';
      var $tourLink = $(tourLink);
      if (!$tourLink.length) {
        return;
      }
      if (typeof hopscotch == 'undefined') {
        return;
      }

      $tourLink.attr('href', '#');

      // Adds our tour overlay behavior with desired effects.
      $(tourLink, context).once('osTourHome', function() {
        $(this).wrap("<div class='os-tour-home-wrapper'></div>");
        $(this).click(function() {
          //$('.hopscotch-bubble').addClass('animated');
          // Sets up the tour object with the loaded feed item steps.
          var tour = {
            showPrevButton: true,
            scrollTopMargin: 100,
            id: "os-tour-home",
            steps: [
              {
                title: 'Welcome!',
                content: 'In this short tour, you\'ll learn all about how to build and manage your site.',
                target: document.querySelector('#os-tour-home'),
                placement: "left"
              },
              {
                title: 'Home: You are here',
                content: 'You can always go back home by clicking on this icon.',
                target: document.querySelector('#toolbar-home'),
                placement: "left"
              },
              {
                title: 'Content: The master list',
                content: 'Every single post of content on your site can be found here.',
                target: document.querySelector('#toolbar-menu'),
                placement: "left"
              },
              {
                title: 'Build: The master list',
                content: 'Every single post of content on your site can be found here.',
                target: document.querySelector('#toolbar-menu'),
                placement: "left"
              },
            ]
          };
          hopscotch.startTour(tour);
          // Removes animation for each step.
          //$('.hopscotch-bubble').removeClass('animated');
          // Allows us to target just this tour in CSS rules.
          //$('.hopscotch-bubble').addClass('os-tour-cp-build-layout');
        });
        $tourLink.slideDown('slow');
      });

    }

  };

})(jQuery);
