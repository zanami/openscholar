(function ($) {

  Drupal.behaviors.os_reader = {
    attach: function (context, settings) {


      $('.feed-item-title').click(function() {
        $(this).siblings('div').children(' .feed-item-description').toggle();
      });


    }
  }
})(window.jQuery);
