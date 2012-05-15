/**
 * 
 */
(function ($) {
  Drupal.behaviors.cp_layout = {
      attach: cp_layout_attach
  };
  
  function cp_layout_attach(ctx) {
    if (ctx == document) {
      cp_layout_init();
    }
    else {
      cp_layout_process_ajax(ctx);
    }
  }
  
  var $regions,
    sort_opts;
  
  /**
   * Called when the page first loads
   * Sets up the sortables and any other behaviors we have
   */
  function cp_layout_init() {
    // things to do:
    // setup dragging regions
    $regions = $('.cp-region').not(':has(.cp-region)');
    var region_ids = [];
    $regions.each(function () {
      region_ids.push('#'+this.id);
    });
    sort_opts = {
      appendTo: '#cp-layout-full-form',
      helper: 'clone',
      cursorAt: {top: 25, left: 38},
      connectWith: region_ids.join(', '),
      stop: on_stop,
      tolerance: 'pointer',
      forceHelperSize: true,
      over: function(event, ui) {
        $(event.target).addClass('active');
      },
      out: function(event, ui) {
        $(event.target).removeClass('active');
      }
    };
    
    $regions.each(function () {
      $(this).sortable(sort_opts);
    });
    
    $('#edit-layout-unused-widgets').sortable(sort_opts);
    
    $('#cp-layout-full-form').submit(cp_layout_submit);
  }
  
  /**
   * Called when we get something through ctools' ajax mechanism
   */
  function cp_layout_ajax(ctx) {
    
  }
  
  /**
   * Saves the form state before submission
   */
  function cp_layout_submit() {
    
    // loop through each region and take note of its widgets
    $regions.each(function () {
      var $input = $(this).find('input'),
          $widgets = $(this).find('.cp-layout-widget'),
          i = 0, l = $widgets.length,
          data = [];
      
      for (;i<l;i++) {
        data.push($widgets[i].id);
      }
      
      // put the list of widgets into the hidden input element in each region
      $input.val(data.join('|'));
    });
    
    // TODO: Discuss if we want to have the form when the user leaves the page via a link.
  }
  
  /**
   * Do things when we drop a widget
   */
  function on_stop() {
    
  }
})(jQuery);