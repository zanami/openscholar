/**
 * 
 */
(function ($) {
  Drupal.behaviors.cp_layout = {
      attach: vsite_layout_attach,
      detach: vsite_layout_detach
  };
  
  function vsite_layout_attach(ctx) {
    if (ctx == document) {
      vsite_layout_init();
    }
    else {
      vsite_layout_process_ajax(ctx);
    }
  }
  
  var $regions,
    sort_opts;
  
  function vsite_layout_init() {
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
  }
  
  function vsite_layout_ajax(ctx) {
    
  }
  
  function vsite_layout_detach() {
    // save form state before its submitted
  }
  
  function on_stop() {
    
  }
})(jQuery);