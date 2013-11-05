(function ($) {

  Drupal.behaviors.optionsSelect2Widget = {

    attach: function (context, settings) {

      var elements = Drupal.settings.select2_widget;
      var element;

      for (var key in elements) {
        element = elements[key];

        var selector = element.selector;
        var cardinality = element.cardinality;

        $(selector).select2({
          maximumSelectionSize: cardinality
        });
      }

    }
  }

})(jQuery);

