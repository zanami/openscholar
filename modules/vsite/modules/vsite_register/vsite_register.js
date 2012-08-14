/**
  * @file vsite_register.js
  *
  * Provides events to change display of select current user/create new user in vsite registration form.
  */

(function($) {
  Drupal.behaviors.vsite_register = {
		attach: function(ctx) {
			var new_user = $('#new_user_div').detach();
			  
			$('#new-user-link').click(function() {
			  $('input[name=create_new_user]').attr('value', 1);
			  
				$existing = $('#edit-existing-username');
			  name = $existing.attr('value');
			  $existing.attr('value', '');
			  $existing.parent().hide();
			  
			  new_user.appendTo('#edit-user-fieldset');
			  $editname = $('#edit-name');
			  if (!$editname.attr('value')) {
			  	$editname.attr('value', name);
			  }
			  $(this).unbind('click');
			});
			
			if (typeof Drupal.settings.vsite_register != 'undefined' && Drupal.settings.vsite_register.new_user_clicked) {
				$('#new-user-link').click();
			}
		}
  };
})(jQuery);