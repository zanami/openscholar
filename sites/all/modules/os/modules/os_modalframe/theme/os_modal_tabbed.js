/**
 * 
 */
$(document).ready(function () {
	// initialize the tabs and the sortable
	$('#tabs').tabs({
				// keep the new_tab link from being a tab
				disabled: [$('#tabs .links li').length-1],
				// use fieldset instead of div
				panelTemplate: '<fieldset class="panel"></fieldset>',
				selected: $('#edit-default').val(),
			  })
			  .find('.ui-tabs-nav')
			  .sortable({
				axis: 'x',
				// don't let them sort the new_tab
				items: 'li:not(.new_tab)',
				update: updateWeight,
			  });
	
	// update the weights of the tabs
	// should end up being unique
	function updateWeight() {
		$('#tabs .links:not(.new_tab) a').each(function (i) {
			$(this.href).find('.tab-weight').val(i);
		});
	}
	
	// set which tab displays by default
	$('.panel .make-default').live('click', function () {
		$('#edit-default').val($(this).attr('panel'));
		$(this).parents('.panel').addClass('default');
	});
	
	// update the title of the tab
	$('.panel .tab-title').live('change', function () {
		var $this = $(this),
			id = $this.parents('.panel').attr('id');
		$('#tabs .links a[href="#'+id+'"]').text($this.val());
	});
	
	// add a new tab when the new_tab is clicked on
	$('#tabs .links .new a').click(function () {
		var id = 'tab-'+($('#tabs .links li').length-1);
		$('#tabs').tabs('add', '#'+id, 'New Tab', $('#tabs .links li').length-1);

		// get the full html, including tags of the panel 
		// and replace the automatically generated panel with it
		$('#'+id).html($('#tab-new').html().replace(/tab-new/g, id));
		$('#tabs').tabs("select", id);
		var count = $('#edit-tab-count');
		count.val(parseInt(count.val())+1);
		
		updateWeight();
	});
	
	// prevent window from expanding to huge heights due to space panels take up
	if (Drupal.modalFrameChild) {
		Drupal.modalFrameChild.triggerParentEvent('childResize');
	}
});