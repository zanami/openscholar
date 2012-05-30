<div id="<?php echo $bid; ?>" class="<?php echo implode(' ', $classes_array); ?>">
  <?php echo $widget_title; ?>
    <div class="widget-controls">
      <?php
      if ($can_edit) {
        print ctools_modal_text_button("Edit", $edit_path, "Edit widget", "edit ctools-modal-openscholar-style-sefault");
      }

      if ($can_delete) {
        print ctools_modal_text_button("Delete", $delete_path, "Delete widget", "delete ctools-modal-openscholar-style-default");
    } ?>
    <div class="close-this" title="Remove">Remove</div>
  </div>
</div>