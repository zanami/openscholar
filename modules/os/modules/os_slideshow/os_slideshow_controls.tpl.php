<?php 
/**
 * Elements for flexslider to use as controls and pager 
 */
?>

<ul id="flex-controls">
  <?php if ($node_count > 1): ?>

  <li id="flex-prev">prev</li>
  <li id="flex-pause">pause</li> 
  <ul id="flex-pager">
    <?php for ($i = 0; $i<$node_count; $i++): ?>
      <li id="flex-page-<?php echo $i;?>" class="flex-page-link"><?php echo $i;?> </li>
    <?php endfor; ?>
  </ul>
  <li id="flex-next">next</li>

  <?php endif; ?>  
</ul>
