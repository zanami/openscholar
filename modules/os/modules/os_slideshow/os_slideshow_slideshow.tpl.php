<?php 
/*
 * Flexslider slides and controls
 */
?>

<div class="flexslider">
  <ul class="slides">
    <?php print implode("\n", $slides); ?>
  </ul>
  
  <?php print $controls; ?>
</div>