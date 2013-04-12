(function ($) {
  Drupal.behaviors.hwpiRemoveStuff = {
    attach: function(context) {
      $('ul.ui-tabs-nav').removeClass('ui-helper-clearfix');
      //$('ul.ui-tabs-nav li a').wrapInner('<span>');
    }
  };
})(jQuery);
