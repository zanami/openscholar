/**
 * Modifies the Software Project node form.
 * 
 * Only shows the Repository URL field if "Manual Upload" is not selected
 */
(function($){
  Drupal.behaviors.os_software = {
    attach: function(context){
      var inputSelector = '#edit-field-software-method-und';
      var selectedValue = $(inputSelector + ' option:selected').val();
      var urlFieldSelector = '#field-software-repo-add-more-wrapper';
      
      // Toggles Repo URL visible or hidden based on new selection
      $(inputSelector).change(function(){
        var newValue = $(this).val().toLowerCase();
        if (newValue === 'manual upload') {
          $(urlFieldSelector).hide();
        } else {
          $(urlFieldSelector).show();
        }
      }).trigger('change'); // Triggers change to initialize field visibility
    }
  };
})(jQuery);