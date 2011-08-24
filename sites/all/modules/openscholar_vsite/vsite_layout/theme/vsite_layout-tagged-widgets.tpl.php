<?php
/**
 *  template for theming a list of widgets
 *  Variables:
 *  ----------
 *  $wgts -> list of all the widgets (dpm($wgts) for more info)
 *  $wgts_id -> the id of the ul
 *  $wgts_class -> the class of the ul
 */
$s_tagged_output = array('Misc' => "");
foreach($wgts as $s_widget_key => $w){
	if(is_array($w['tags']) && count($w['tags'])){
		foreach ($w['tags'] as $s_tag) {
			$s_tagged_output[$s_tag] .= theme('vsite_layout_cp_widget', $s_widget_key, $w);
		}
	}else{
		$s_tagged_output['Misc'] .= theme('vsite_layout_cp_widget', $s_widget_key, $w);
	}
}
$s_tagged_output = array_reverse($s_tagged_output);
?>
<div class="<?php print $wgts_id; ?>-wrapper">
<ul id="widget-categories">
<?php
  foreach($s_tagged_output as $s_tag_name => $output){
  	$s_tag_id = strtolower( str_replace(' ', '_', $s_tag_name));
    echo "<li><a href=\"#{$s_tag_id}\">{$s_tag_name}</a></li>";
  }
?>
</ul>

<dl id="<?php print $wgts_id; ?>" class = "<?php print $wgts_class; ?>">
  <dt><?php print $region_name;?></dt>
	<?php
	echo implode(" ",$s_tagged_output);
	?>
</dl>
</div>