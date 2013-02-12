<?php 
/**
 * Elements for flexslider to use as controls and pager 
 */
?>

<div id="flex-controls">
  <span id="flex-prev">prev</span>
  <span id="flex-pause">pause</span> 
  <div id="flex-pager">
    <?php for ($i = 0; $i<$node_count; $i++): ?>
      <span id="flex-page-<?php echo $i;?>" class="flex-page-link"> </span>
    <?php endfor; ?>
  </div>
  <span id="flex-next">next</span>
</div>
