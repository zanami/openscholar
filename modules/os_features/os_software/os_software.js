/**
 * Modifies the Software Project node form.
 * 
 * Only shows the Repository URL field if "Manual Upload" is not selected
 */
(function($){
  
  Drupal.behaviors.os_software = {
    attach: function (context) {
      var input_selector = '';
      var url_field_selector = '';
      $(url_field_selector).hide();
      $(input_selector).change(function(){
    	  manual_upload = ((this).val() === 'manual_upload');
    	  if (manual_upload) {
    		  $(url_field_selector).hide();
    	  } else {
    		  $(url_field_selector).show();
    	  }
      });
    }
  };
})(jQuery);