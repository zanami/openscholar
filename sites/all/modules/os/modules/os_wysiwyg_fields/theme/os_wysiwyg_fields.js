/**
 * Overrides some of wysiwyg_fields' functions to be more intuitive
 */
(function ($) {
	if (typeof Drupal.wysiwyg == 'undefined')
		return;
	
	function cleanUp(id) {
		// remove the 'Expand' button. There's nothing hidden to Expand.
		// well, there is, but there's no reason it should be hidden to start.
		var btn = $('#wysiwyg_fields-' + id + '-dialog .ui-dialog-titlebar .wysiwyg_fields-icon-expand').hide();
		if (id == 'field_os_inline_oembed') {
			btn.click();
		}
		
		// remove the Insert button added by wysiwyg_fields.
		// The button provided by Insert is more useful.
		$('#wysiwyg_fields-' + id + '-dialog .ui-dialog-buttonpane').not('#wysiwyg_fields-field_os_inline_oembed-dialog .ui-dialog-buttonpane').remove();
		
		// Show the table
		// $('#' + id.replace('_', '-', 'g') + '-items, #wysiwyg_fields-' + id + '-wrapper table').show();
		
		var moved = $('.wysiwyg_fields-field:not(table .wysiwyg_fields-field)');
		$('#'+moved.attr('id')+'-placeholder').before(moved);
		
		// sets the wrapperElement to something other than span
		// if its span, oembed objects would get popped out and
		// wouldnt be replaced properly on detach.
		Drupal.wysiwygFields.wrapperElement = wrapperElement = 'form';
	}
	
	// store the current functions
	var dialogShowDefaultOld = Drupal.wysiwygFields.dialogShowDefault,
		dialogShowUpdateOld = Drupal.wysiwygFields.dialogShowUpdate;
	
	// overwrite current functions with new ones that call the current funcs and tweak the results.
	Drupal.wysiwygFields.dialogShowDefault = function (id) {
		dialogShowDefaultOld.call(Drupal.wysiwygFields, id);
		cleanUp(id);
	}
	
	Drupal.wysiwygFields.dialogShowUpdate = function (id) {
		dialogShowDefaultUpdate.call(Drupal.wysiwygFields, id);
		cleanUp(id);
	}
	
	var hasRun = false;
	Drupal.behaviors.adjustWysiwygSettings = function () {
		
		Drupal.settings.wysiwyg.enable = 'Rich-text Editor View';
		Drupal.settings.wysiwyg.disable = 'HTML View';
		
		// set extended valid settings to include id for spans
		// prevents error with inserted items ending up in helper elements that are removed later
		// run this every time incase an ahah call replaces it
		var settings = Drupal.settings.wysiwyg.configs.tinymce;
		
		if (typeof settings.format1 == 'object')
			settings = settings.format1;
		else if (typeof settings.format6 == 'object')
			settings = settings.format6;
		else if (typeof settings.format5 == 'object') 
			settings = settings.format5;
		else return;
		
		var eve = settings.extended_valid_elements.split(',');
		jQuery.each(eve, function(i, item) {
			if (item.indexOf('span') != -1 && item.indexOf('id') == -1) {
				var t = item;
				t = t.replace('span[', '').replace(']','').split('|');
				t.push('id');
				eve[i] = 'span['+t.join('|')+']';
			}
		});
		eve.push('iframe[src|href]');
		eve.push('form[id|class|contenteditable]');
		settings.extended_valid_elements = eve.join(',');
		
		
		// change some vars and functions so oembed stuff doesn't pop out of the span
		Drupal.wysiwygFields.wysiwyg.tinymce.wrapperElement = 'form';
//		Drupal.wysiwygFields.wysiwyg.tinymce.divToWysiwygField = function() {
//			delete Drupal.settings.wysiwygFields.timer;
//			if (typeof tinyMCE.activeEditor.contentDocument !== "undefined") {
//			  $('.wysiwyg_fields-placeholder', tinyMCE.activeEditor.contentDocument.body).each(function() {
//				var $self = $('#'+this.id, tinyMCE.activeEditor.contentDocument.body), replacement;
//				$self.removeClass('wysiwyg_fields-placeholder');
//			    replacement = "<"+Drupal.wysiwygFields.wrapperElement+" id='" + $self.attr('id') + "' class='" + $self.attr('class') + "' contenteditable=\"false\">" + Drupal.settings.wysiwygFields.replacements['[' + $self.attr('id') + ']'] + "</"+Drupal.wysiwygFields.wrapperElement+"> ";
//			    Drupal.wysiwygFields.wysiwyg.tinymce.selectNode($self[0]);
//			    Drupal.wysiwyg.instances[Drupal.settings.wysiwygFields.activeId].insert(replacement);
//			  });
//			}
//			else {
//			  // Document not ready, reset timer.
//			  Drupal.wysiwygFields._wysiwygAttach();
//			}
//	    };
	    
		// pull the Insert button out of a div and next to remove
		
		if (hasRun) {
			$('.widget-edit:visible').each(function (item) {
				var btn = this.getElementsByClassName('insert-button')[0];
				if (!btn) return;
				btn.parentNode.removeChild(btn);
				
				$('input[value="Remove"]', this).before(btn);
			});
		}
		else {
//			var insert = Drupal.wysiwyg.editor.instance.tinymce.insert;
//			Drupal.wysiwyg.editor.instance.tinymce.insert = function(content) {
//				content += ' ';
//				if (typeof tinyMCE.activeEditor.contentDocument !== 'undefined' 
//					&& !$('p', tinyMCE.activeEditor.contentDocument.body).length) {
//					content += ' <p></p>';
//				}
//				insert.call(this, content);
//			};
			
			// prevent links in oembed code from going anywhere
			tinyMCE.onAddEditor.add(function () {
				tinyMCE.activeEditor.onClick.add(function (ed, e) {
					var targ = e.target || e.srcElement;
					while (targ.contentEditable == 'inherit') {
						targ = targ.parentNode;
					}
					if (targ.contentEditable == 'false') {
						this.selection.select(targ);
						e.preventDefault();
						return false;
					}
				});
			});
			/*
			if (typeof Drupal.wysiwyg.plugins.wysiwyg_fields_field_os_inline_oembed == 'object') {
				var init = Drupal.wysiwyg.plugins.wysiwyg_fields_field_os_inline_oembed.init;
				Drupal.wysiwyg.plugins.wysiwyg_fields_field_os_inline_oembed.init = function (content, settings, instanceId) {
					init.call(this);
					var body = tinyMCE.activeEditor.contentDocument.body;
					if ('addEventListener' in body) {
						body.addEventListener('click', , false);
					}
					else {
						body.attachEvent('onClick', function (e) {
							var targ = e.target;
							while (targ.contentEditable == 'inherit') {
								targ = targ.parentNode;
							}
							if (targ.contentEditable == 'false') return false;
						});
					}
				};
			}*/
		}
		hasRun = true;
	};
})(jQuery);