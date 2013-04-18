<?php 
/*
 * Flexslider slides and controls
 */
?>
<div class="<?php echo 'layout_' . $layout; ?>">
	<div class="os_slideshow">
	<div class="flexslider">
	 <ul class="slides">
      <?php print implode("\n", $slides); ?>
    </ul>
  
    <?php print $controls; ?>
  </div>
</div>
</div>