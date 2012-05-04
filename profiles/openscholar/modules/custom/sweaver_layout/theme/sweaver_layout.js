(function ($) {
Drupal.behaviors.sweaver_layout = {
	attach: function (ctx) {
		// don't do this multiple times
		var $panel = $('#sweaver_layout', ctx);
		if ($panel.length && !$panel.hasClass('.swl-processed')) return;
		$panel.addClass('swl-processed');
		
		$('#sweaver-tabs a').not('#tab-sweaver_layout a').click(disableEditing);
		$('#tab-sweaver_layout a').click(enableEditing);
	}
};

function enableEditing() {
	
}

function disableEditing() {
	
}
})(jQuery);