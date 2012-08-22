/**
 * Modifies the Software Project node form.
 * 
 * Only shows the Repository URL field if "Manual Upload" is not selected
 */
(function($){
  Drupal.behaviors.os_software = {
    attach: function(context){
      var inputSelector = '#edit-field-software-method-und';
      var urlFieldSelector1 = '#field-software-repo-add-more-wrapper';
      var urlFieldSelector2 = '#field-rbuild-short-name-add-more-wrapper';
      // Toggles Repo URL visible or hidden based on new selection
      $(inputSelector).change(function(){
        var newValue = $(this).val().toLowerCase();
        if (newValue !== 'rbuild repository') {
          $(urlFieldSelector1).hide();
          $(urlFieldSelector2).hide();
        } else {
          $(urlFieldSelector1).show();
          $(urlFieldSelector2).show();
        }
      }).trigger('change'); // Triggers change to initialize field visibility
    }
  };
})(jQuery);