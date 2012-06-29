/**
 */
(function($) {

Drupal.behaviors.box_slideshow = {
	attach: function (ctx) {
		$('#edit-images-fieldset').mouseup(function() {
			
			alert('asdf');
		})
	}
};

}(jQuery));