(function ($) {
  Drupal.behaviors.osReadOnlyModeAlert = {

    attach: function (context, settings) {


      $("#edit-os-readonly-mode").click(function() {
        if ($(this).is(':checked')) {
          $("#readonly-dialog").dialog(
            {
              modal: true,
              height: 400,
              width: 600
            }
          );
        }
        else {
          $("#readonly-dialog").dialog('close');
        }
      });
    }
  }
})(jQuery);
