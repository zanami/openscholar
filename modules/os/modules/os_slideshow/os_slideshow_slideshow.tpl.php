<?php 
/*
 * Flexslider slides and controls
 */
?>

<div id="<?php echo $layout; ?>">
  <ul class="rslides">
    <?php print implode("\n", $slides); ?>
  </ul>
  
  <?php print $controls; ?>
</div>
