(function ($) {
  /**
   *
   */
  Drupal.wysiwygFields = Drupal.wysiwygFields || {};
  Drupal.wysiwygFields.wysiwyg = Drupal.wysiwygFields.wysiwyg || {};

  /**
   *
   */
  Drupal.wysiwygFields.wysiwyg.tinymce = {
    // Wrapper element override.
    wrapperElement: 'span',

    init: function(id) {
      // MCEditor icon size fix.
      $('.mce_wysiwyg_fields_' + id).addClass('mce_wysiwyg_fields_icon');
      
    },

    /**
     * @TODO - Remove IMG resize helper.
     */
    wysiwygIsNode: function(element) {
      var editor = tinyMCE.activeEditor;

      // Create the range for the element.
      var range = editor.contentDocument.createRange();
      range.selectNode(element);

      // Select the range.
      var sel = editor.selection.getSel();
      if (sel.containsNode(element)) {
	      sel.removeAllRanges();
	      sel.addRange(range);
      }
    },
    
    selectNode: function(element) {
    	var editor = tinyMCE.activeEditor,
    		range = editor.contentDocument.createRange(),
    		sel = editor.selection.getSel();
    	
    	range.selectNode(element);
    	sel.removeAllRanges();
    	sel.addRange(range);
    },

    /**
     *
     */
    divToWysiwygField: function() {
      delete Drupal.settings.wysiwygFields.timer;
      var elem = this.wrapperElement;
      if (typeof tinyMCE.activeEditor.contentDocument !== "undefined") {
        $(elem+'.wysiwyg_fields-placeholder', tinyMCE.activeEditor.contentDocument.body).each(function() {
          var $self = $('#'+this.id, tinyMCE.activeEditor.contentDocument.body),
          	replacement;
          $self.removeClass('wysiwyg_fields-placeholder');
          replacement = "<"+elem+" id='" + $self.attr('id') + "' class='" + $self.attr('class') + "' contenteditable=\"false\">" + Drupal.settings.wysiwygFields.replacements['[' + $self.attr('id') + ']'] + "</"+elem+">";
          Drupal.wysiwygFields.wysiwyg.tinymce.selectNode($self[0]);
          Drupal.wysiwyg.instances[Drupal.settings.wysiwygFields.activeId].insert(replacement);
        });
      }

      else {
        // Document not ready, reset timer.
        Drupal.wysiwygFields._wysiwygAttach();
      }
    }
  }
})(jQuery);
