<?php 
/*
 * Flexslider slides and controls
 */
?>

<div id="<?php echo $layout; ?>" class="flexslider">
  <ul class="slides">
    <?php print implode("\n", $slides); ?>
  </ul>
  
  <?php print $controls; ?>
</div>
