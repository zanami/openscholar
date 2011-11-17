/**
 * Allows users to add posts to their manual lists without an additional 
 * page load on top of the ajax call
 */
Drupal.behaviors.os_manual_list = function (ctx) {
	if ($('#manual-nodes-list', ctx).length == 0) return;	// do nothing if our table doesn't exist
	
	var $form = $('#boxes-box-form'),
		template = '<tr class="draggable">'+$('#edit-nodes-blank-nid').parents('tr').hide().html()+'</tr>',
		tableDrag = Drupal.tableDrag['manual-nodes-list'];
	
	// add a new row to the table, set all its form elements to the right values and make it draggable
	$('.add_new', $form).click(function (e) {
		var val = $('#edit-node-to-add', $form).val(),
			patt = /(.+) \[nid:([\d]+)\]/,
			matches = patt.exec(val),
			id = $('table tr', $form).length-1,
			new_row = $(template.replace(/blank/g, id));
		
		// there should actually be something in the field
		if (matches != null) {
			var count = $('#edit-count');
			count.val(parseInt(count.val())+1);
			
			// set all the form elements in the new row
			$('#edit-nodes-'+id+'-nid', new_row).val(matches[2]);
			$('span', new_row).text(matches[1]);
			$('#edit-nodes-'+id+'-title', new_row).val(matches[1]);
			$('#edit-nodes-'+id+'-weight', new_row).addClass('field-weight').val(id);
			$('.tabledrag-handle', new_row).remove();
			$('table tbody', $form).append(new_row);
			new_row = $('#edit-nodes-'+id+'-nid', $form).parents('tr');
			$('#edit-node-to-add', $form).val('');
			
			setup_remove(new_row);

			tableDrag.makeDraggable(new_row[0]);
			tableDrag.hideColumns();
		}
	});
	
	// set up remove links.
	function setup_remove(ctx) {
		$('.remove', ctx).click(function () {
			var $this = $(this);
			$this.parents('tr').remove();
			
			// decrement counter
			var count = $('#edit-count');
			count.val(parseInt(count.val())-1);
			
			return false;
		});
	}
	
	setup_remove($form);
}