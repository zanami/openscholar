<?php
/*
 * Flexslider slides and controls
 */
?>

<div class="flexslider <?php echo $layout; ?>">
  <ul class="slides">
    <?php print implode("\n", $slides); ?>
  </ul>

  <?php print $controls; ?>
</div>