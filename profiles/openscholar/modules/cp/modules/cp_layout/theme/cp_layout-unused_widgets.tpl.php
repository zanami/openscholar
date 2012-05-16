<div id="edit-layout-unused-widgets">
<!-- Do something about tabs and factories -->
<?php
echo $factory_html;
if (count($tags) > 0) { ?>
	<ul><?php
  foreach ($tags as $t) {
    ?>
    <li><a href="#<?php echo $t; ?>"><?php echo $t; ?></a></li>
    <?php
  }
  ?></ul><?php
}
echo $children;
?>
</div>