/**
 * 
 */
Drupal.behaviors.filterTipsOnWysiwygDetach = function (ctx) {
	$('.tips', ctx).hide();
	$('#wysiwyg-toggle-edit-body', ctx).live('click', function (e) {
		$('.tips', ctx).toggle();
	});
}