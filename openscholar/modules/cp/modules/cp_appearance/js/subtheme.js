(function ($) {

  Drupal.behaviors.hideNoAjac = {
    attach: function () {
      // When AJAX is available, hide the no js option from the select list.
      $("#edit-branch option[value='nojs']").each(function() {
        $(this).remove();
      });
    }
  }

})(jQuery);
