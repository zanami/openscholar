(function ($) {
  Drupal.behaviors.hwpiRemoveStuff = {
    attach: function(context) {
      $('ul.ui-tabs-nav').removeClass('ui-helper-clearfix');
    }
  };
})(jQuery);
