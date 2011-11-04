/**
 * 
 */
$(document).ready(function () {
	$('#tabs').tabs({
				disabled: [$('#tabs .links .new').index()]
			  })
			  .find('.ui-tabs-nav')
			  .sortable({
				axis: 'x',
				items: 'li:not(.new_tab)',
				update: updateWeight,
			  });
	
	// prevent window from expanding to huge heights due to space panels take up
	if (Drupal.modalFrameChild) {
		Drupal.modalFrameChild.triggerParentEvent('childResize');
	}
	
	function updateWeight() {
		$('#tabs .links:not(.new_tab) a').each(function (i) {
			$(this.href).find('.tab-weight').val(i);
		});
	}
	
	$('.panel .make-default').click(function () {
		console.log('click default');
		$('#edit-default').val($(this).attr('panel'));
	});
	
	$('#tabs .links .new a').click(function () {
		console.log('click new');
		$('#tabs').tabs('add', '#test1', 'New Tab', $('#tabs .links .new').index());
	});
});