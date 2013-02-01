/**
 * Attach event to remove button in slideshow box form.
 */
Drupal.behaviors.os_slideshow_box = {
	attach : function (ctx) {
	  
		var $ = jQuery;
		
		/* Events for remove links */
    var $form = $('#boxes-add-form, #boxes-box-form');

		// set up remove links.
		function setup_remove(ctx) {
			$('.remove', ctx).click(function () {
				var $this = $(this);
				$this.parents('tr').remove();							
				return false;
			});
		}
		
		setup_remove($form);
		
		
		/* Slider */
    var input = $('#edit-size', ctx);
    if (input.length == 0) {
      return;
    }
    input[0].type = "range";
    if (input.attr('type') == 'range') {
      // this browser supports the new form input types
      // set the rest of them
      input.attr(Drupal.settings.os_slideshow_box);
      // and display the actual pixel width as the user changes it
      input.change(function (e) {
        input.parent().find('.field-suffix').html(input.val()+"px");
      });
      input.change();
    }
  }
};