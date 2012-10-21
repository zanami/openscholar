jQuery(document).ready(function() {
//Insert the secondary nav to the content region for smartphone
 jQuery(window).resize(function() {
  if (jQuery(window).width() < 600) {
     jQuery("#block-os-secondary-menu").insertAfter(".region-sidebar-second");
  }

 });

});
