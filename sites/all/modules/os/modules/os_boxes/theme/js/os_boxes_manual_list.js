/**
 * Allows users to add posts to their manual lists without an additional 
 * page load on top of the ajax call
 */
Drupal.behaviors.os_manual_list = function (ctx) {
	if ($('#manual-nodes-list', ctx).length == 0) return;	// do nothing if our table doesn't exist
	
	var $form = $('#boxes-box-form'),
		template = '<tr>'+$('#edit-nodes-blank-nid').parents('tr').hide().html()+'</tr>';
	
	$('.add_new', $form).click(function (e) {
		var val = $('#edit-node-to-add', $form).val(),
			patt = /(.+) \[nid:([\d]+)\]/,
			matches = patt.exec(val),
			id = $('table tr', $form).length-1,
			new_row = $(template.replace(/blank/g, id)).wrap('<tr />');
		
		if (matches != null) {
			$('#edit-nodes-'+id+'-nid', new_row).val(matches[2]);
			$('span', new_row).text(matches[1]);
			$('#edit-nodes-'+id+'-title', new_row).val(matches[1]);
			$('table tbody', $form).append(new_row);
			new_row = $('#edit-nodes-'+id+'-nid', $form).parents('tr');
			$('#edit-node-to-add', $form).val('');

			Drupal.tableDrag['manual-nodes-list'].makeDraggable(new_row[0]);
		}
	});
}