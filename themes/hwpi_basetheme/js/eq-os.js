(function ($) {
  Drupal.behaviors.eqOS = {
    attach: function(context) {
      if (matchMedia('only screen and (min-width: 1025px)').matches) {
      
        $('#content-panels .region-content-first .region-inner,#content-panels .region-content-second .region-inner').equalHeight();

      }
    }
  };
})(jQuery);
