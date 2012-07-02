
 
(function($) {

Drupal.behaviors.box_slideshow = {
	attach: function (ctx) {
		

		function updateSelected() {
			//clear old css
			$('.slideshow-image').removeClass('slideshow-image');
			
			//add new
			fids = getAllFIDS();
			alert(fids);
			for (fid in fids) {
				$('a[data-fid=' + fids[fid] + '] > div').parent().addClass('slideshow-image')
			}
			
		}
		
		//returns array with values of 'value' stripped out
		function filter_array(array, value) {
			return jQuery.grep(array, function(a) {return a != value;})
		}
		
		function getAllFIDS() {
			csv = $('#edit-image-fids').attr('value');
			//return $.parseJSON(json);
			return csv.split(',')
		} 
		
		function setAllFIDS(fids) {
			fids = jQuery.unique(fids);
			fids = filter_array(fids, '');
			csv = fids.join(',');
			$('#edit-image-fids').attr('value', csv)
		}
		
		function getSelectedFID() {
			return $('div.selected').parent().attr('data-fid');
		}
		
		function addImage() {
			fid = getSelectedFID();
			fids = getAllFIDS();
			fids.push(fid);
			setAllFIDS(fids);
		}
		
		function removeImage(fid) {
			fids = getAllFIDS();
			fids = filter_array(fids, fid);
			setAllFIDS(fids);
		}
		
		
		
		//init
		// add button events
		// add classes for currently selected
		$('#edit-add-selected-image').click(function() {
			//update something to hide?  not sure how to check css
			addImage();
			updateSelected();
		});
		
		$('#edit-remove-selected-image').click(function() {
			removeImage(getSelectedFID());
			updateSelected();
		});
		
		updateSelected();

	}
}
}(jQuery));
