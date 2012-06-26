/**
  * @file vsite_register.js
  *
  * Provides events to toggle display of select current user/create new user in vsite registration form.
  */

(function($) {
  Drupal.behaviors.vsite_register = {
		attach: function(ctx) {
			var new_user = $('#new_user_div').detach();
			  
			$('#new-user-link').click(function() {
			  $('.register-existing-user').parent().hide(); 
			  new_user.appendTo('#edit-user-fieldset');
			  $(this).unbind('click');
			});
		}
  };
})(jQuery);