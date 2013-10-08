(function ($) {

  Drupal.behaviors.submitOnChange = {
    attach: function () {
      $(function() {
        $('.terms-list').change(function(e) {
          $(this).closest('form').trigger('submit')
        })
      })
    }
  };

})(jQuery);
