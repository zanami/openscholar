/**
 * jQuery behaviors for platform notification feeds.
 */
(function ($) {

Drupal.behaviors.os_notifications = {

attach: function (context, settings) {

  // Setup.
  var tourLink = '#os-tour-cp-build-layout';
  var $tourLink = $(tourLink);
  if (!$tourLink.length) {
    return;
  }
  if (typeof hopscotch == 'undefined') {
    return;
  }

  $tourLink.attr('href', '#');

  // Adds our tour overlay behavior with desired effects.
  $(tourLink, context).once('osTourCpBuildLayout', function() {
    $(this).click(function() {
      //$('.hopscotch-bubble').addClass('animated');
      // Sets up the tour object with the loaded feed item steps.
      var tour = {
        showPrevButton: true,
        scrollTopMargin: 100,
        id: "os-tour-notifications",
        steps: [
          {
            title: 'Welcome!',
            content: 'In this short tour, you\'ll learn all about layout and widgets.',
            target: document.querySelector('#os-tour-cp-build-layout'),
            placement: "left"
          },
          {
            title: 'Site sections',
            content: 'Click here to see all the App sections enabled on your site.',
            target: document.querySelector("#edit-context-selection"),
            placement: "right",
            yOffset: -20,
            onShow: function () {$('#edit-context-selection').focus();}
          },
          {
            title: 'Drag \'n\' drop!',
            content: 'This diagram shows you the current layout for the selected section. You can drag things around to your heart\'s content!',
            target: document.querySelector("#edit-layout"),
            placement: "top"
          },
          {
            title: 'Missing something?',
            content: 'Scroll through the widget gallery to see what other existing widgets you might want to drag into your layout.',
            target: document.querySelector("#edit-layout-unused-widgets"),
            placement: "right"
          }
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
