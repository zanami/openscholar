<div id="<?php echo $bid; ?>" class="<?php echo implode(' ', $classes_array); ?>">
	<?php echo $widget_title; ?>
	<div class="widget-controls">
		<?php if ($can_delete) {
			print ctools_modal_text_button("Delete", $w['block_delete_path'], "Delete widget", "delete ctools-modal-openscholar-style-default");
		} ?>
	</div>
</div>