<?php
/**
 *  template for theming a list of widgets
 *  Variables:
 *  ----------
 *  $wgts -> list of all the widgets (dpm($wgts) for more info)
 *  $wgts_id -> the id of the ul
 *  $wgts_class -> the class of the ul
 */
?>
<div class="widget-prev"></div><div class="widget-next"></div>

<dl id="<?php print $wgts_id; ?>" class = "<?php print $wgts_class; ?>">
  <dt><?php print $region_name;?></dt>
	<?php
	foreach($wgts as $s_widget_key => $w){
	  echo theme('vsite_layout_cp_widget', $s_widget_key, $w);
	}
	?>
</dl>


<select id="widget-tag-select">
  <option value='all'>All Widgets</option>
  <?php foreach ($wgts_tags as $title => $class) {echo '<option value="'.$class.'">'.$title.'</option>';}?>
</select>
