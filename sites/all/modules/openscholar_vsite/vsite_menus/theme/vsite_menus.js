Drupal.behaviors.vsiteMenus = function() {
  $("#vsite-menus-settings-form input.edit-tooltip").each(function() {
	  var editLink = $('<a>edit tooltip</a>').click(function(){
		  $(this).siblings('.description').show();
		  $(this).siblings('.edit-tooltip').show();
		  $(this).hide();
	  });
	  $(this).hide();
	  $(this).siblings('.description').hide();
	  $(this).parent("div.form-item").prepend(editLink);
  });
	
};