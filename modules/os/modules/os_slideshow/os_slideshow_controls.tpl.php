<?php
/**
 * Elements for flexslider to use as controls and pager
 */
?>

<ul id="flex-controls">
  <?php if ($node_count > 1): ?>

  <li id="flex-prev"><a class="prev" href="#"><span class="arrow-left">prev</span></a></li>
  <li id="flex-pause"><a class="pause" href="#">pause</a></li>
  <ul id="flex-pager">
    <?php for ($i = 0; $i<$node_count; $i++): ?>
      <li id="flex-page-<?php echo $i;?>" class="flex-page-link"><a class="slide" href="#"><?php echo $i;?> </a></li>
    <?php endfor; ?>
  </ul>
  <li id="flex-next"><a class="next" href="#"><span class="arrow-right">next</span></a></li>

  <?php endif; ?>
</ul>
