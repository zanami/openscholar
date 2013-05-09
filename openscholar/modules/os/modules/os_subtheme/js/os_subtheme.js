/**
 * @file
 * JavaScript file for the OS subtheme module.
 */

(function ($) {
  Drupal.behaviors.groupRefChange = {
    attach: function(context, settings) {
      $(".show-dialog").click(function(event) {
        event.preventDefault();

        // Close any dialog open.
        $("#dialog").dialog("close");

        $("#dialog").dialog({
          autoOpen: false,
          draggable: true,
          closeOnEscape: true,
          title: $(this).attr('title')
        });

        // Open a new dialog.
        $("#dialog").dialog("open");

      });
    }
  }
})(jQuery);


