/*
 * If the TinyMCE wysiwyg files are loaded after the document is ready, it fails to initialize properly
 */
Drupal.behaviors.AAATinyMceInitFailsafe = function() {
	if (typeof tinymce != 'undefined') {
		tinymce.dom.Event._pageInit();
	}
}