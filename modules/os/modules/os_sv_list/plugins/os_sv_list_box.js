/**
 * @file os_sv_list_box.js
 * 
 * When a content type is selected, change the more link to something appropriate, but only if it hasn't been changed.
 **/


(function($) {
  Drupal.behaviors.os_sv_list_box = {
		attach: function(ctx, settings) {
			$('#edit-content-type').change( function() {
				var content_menu = new Array();
        content_menu['all'] = '';
        content_menu['announcement'] = 'announcements';
        content_menu['biblio'] = 'publications';
        content_menu['event'] = 'calendar';
        content_menu['page'] = 'pages';
        content_menu['data'] = 'data';
        content_menu['blog'] = 'blog';
        content_menu['book'] = 'documents';
        content_menu['feed'] = 'reader';
        content_menu['media_gallery'] = 'galleries';
        content_menu['link'] = 'links';
        content_menu['presentation'] = 'presentations';
        content_menu['software_project'] = 'software';
        content_menu['software_release'] = 'software'; 
        content_menu['person'] = 'people';
        
        function in_obj(needle, object) {
        	for (var o in object) {
        		if (object[o] == needle) {
        			return true;
        		}
        	}
        	return false;
        }

        var selected = $(this).attr('value');
        var current_link = $('#edit-more-link').attr('value');

			  //only replace standard links.  a user who specifies "writing" or something should have that preference overriden
				if (in_obj(current_link, content_menu)) {
					var new_link = content_menu[selected];
					$('#edit-more-link').attr('value', new_link);
				}
			})
		}
  }
})(jQuery);
