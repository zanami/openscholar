/**
 * jQuery to toggle form elements for the biblio content type.
 */

(function($) {
  Drupal.behaviors.os_sv_list = {
    attach : function(context) {
      // when content type changes, update sorting options list.
      var old_type = $('#os_sv_list_content_type').val();
      $('#os_sv_list_content_type').change(function() {
        var sortby = $('#edit-sort-by');
        var whitelist = [ 'sort_newest', 'sort_oldest', 'sort_alpha' ];
        var content_type = $('#os_sv_list_content_type').val();
        var selected_sort = 'sort_' + content_type;
        var more_link = $('#edit-more-link');
        var defaults = Drupal.settings.more_link_defaults;

        sortby.children('option').each(function() { // why you no
          this_sort = $(this).attr('value');
          if ($.inArray(this_sort,whitelist) == -1) {
            // show/hide appropriate options
            remove = (this_sort != selected_sort);
            $(this).attr('hidden', remove).attr('disabled',remove);

            // deselect invalidated option
            if (remove && ($(this).parent().attr('value') == this_sort)) {
              $(this).parent().attr('value', whitelist[0]);
            }

            // for new boxes, select special case as default
            if (!remove && Drupal.settings.os_sv_list_box.new_box) {
              $(this).parent().attr('value', this_sort);
            }
          }
        });

        // swap out the more link url
        if (more_link.val()) {
          defaults[old_type] = more_link.val();
        }
        more_link.val(defaults[content_type]);
        old_type = content_type;

      });

      // perform the change callback once now.
      $('#os_sv_list_content_type').change();

      // Get the default value of the content_type.
      var content_type = $('#os_sv_list_content_type').val();
      var show_all_checked = $('#biblio_show_all_check').is(':checked') ? true : false;

    }
  };
}(jQuery));
