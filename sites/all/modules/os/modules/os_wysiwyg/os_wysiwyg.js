/*
 * If the TinyMCE wysiwyg files are loaded after the document is ready, it fails to initialize properly
 */
Drupal.behaviors.AAATinyMceInitFailsafe = function(ctx) {
	if (typeof tinymce != 'undefined') {
		tinymce.dom.Event._pageInit();
	}

	if ($('.wysiwyg', ctx).length) {
		$('#modalContent #edit-submit', ctx).mouseover(function (e) {
			if (tinyMCE) {
				tinyMCE.triggerSave();
			}
		});
	}
}