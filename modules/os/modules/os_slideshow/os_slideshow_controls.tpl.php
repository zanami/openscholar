<?php 
/**
 * Elements for flexslider to use as controls and pager 
 */
?>

<div class="controls-wrap">
  <ol class="flex-control-nav flex-control-paging">
    <?php for($n = 0 ; $n < $node_count; $n++):?>
      <li><a href="#" class="flex-pager <?php echo $n; ?>"><?php echo $n; ?></a></li>
    <?php endfor;?>
  </ol>
  
  <ul class="flex-direction-nav">
    <li><a href="#" class="flex-prev">Previous</a></li>
    <li><a href="#" class="flex-next">Next</a></li>
  </ul>
</div>
