/**
 * Allows users to add posts to their manual lists without an additional 
 * page load on top of the ajax call
 */
Drupal.behaviors.os_slideshow_box = {
	attach : function (ctx) {
		var $ = jQuery;
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
	}
};