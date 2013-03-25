<?php 
/*
 * Flexslider slides and controls
 */
?>

<div class="flexslider <?php echo 'layout_' . $layout; ?>">
	<div class="slideshow">
    <ul class="slides">
      <?php print implode("\n", $slides); ?>
    </ul>
  
    <?php print $controls; ?>
  </div>
</div>