jQuery(document).ready(function() {
  var wide = jQuery(".views_slideshow_slide img").width();
    jQuery('.block-boxes-os_boxes_slideshow > div').attr('width', wide);
    jQuery('.views-slideshow-controls-text').attr('width', wide);
    jQuery('.views_slideshow_cycle_main').attr('width', wide);
    
});