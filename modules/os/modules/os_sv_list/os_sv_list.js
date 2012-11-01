
/**
 * jQuery to toggle form elements for the biblio content type.
 */

(function ($) {
  Drupal.behaviors.os_sv_list = {
    attach: function(context) {
      // Get the default value of the content_type.
      var content_type = $('#os_sv_list_content_type').val(); 
      var show_all_checked = $('#biblio_show_all_check').is(':checked')?true:false;
      
 	  // Check for a select action on the conten_type.
      $('#os_sv_list_content_type', context).change(function() {
        content_type = $(this).val();
      });
      
      // Check for a click event on the show all publication types.
      $('#biblio_show_all_check', context).click(function() {
        if($('#biblio_show_all_check').not(':checked') && content_type == 'biblio') {
          $('#os_sv_list_biblio_whitelist').show(); 
          show_all_checked = false;	
        }
        if($('#biblio_show_all_check').is(':checked')) {
          $('#os_sv_list_biblio_whitelist').hide();
          show_all_checked = true;
        }
      });
      
      // Hide/show the select all pub types based on select value.
      if(content_type == 'biblio') {
   	    $('#os_sv_list_biblio_show_all').show('slow',function(){
   	      if(show_all_checked == false) {
   	    	$('#os_sv_list_biblio_whitelist').show();
   	      }
   	    });
   	  }else {
   		$('#os_sv_list_biblio_show_all').hide();  
   		$('#os_sv_list_biblio_whitelist').hide();
   	  }
	}
  };
}(jQuery));
