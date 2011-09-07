<?php
/**
 *  template for theming a list of widgets
 *  Variables:
 *  ----------
 *  $wgts -> list of all the widgets (dpm($wgts) for more info)
 *  $wgts_id -> the id of the ul
 *  $wgts_class -> the class of the ul
 */
$s_tagged_output = array('Content' => "",'Media' => "",'Social' => "",'Misc' => "");
foreach($wgts as $s_widget_key => $w){
	if(!is_array($w['tags'])) $w['tags'] = array();
	
	//Return the first tag that matches the display categories above
	$s_tag = array_shift(array_intersect(array_keys($s_tagged_output),$w['tags']));
	if($s_tag){
	  $s_tagged_output[$s_tag] .= theme('vsite_layout_cp_widget', $s_widget_key, $w);
	}else{
		$w['tags'][] = "Misc";
		$s_tagged_output['Misc'] .= theme('vsite_layout_cp_widget', $s_widget_key, $w);
	}
}

?>
<div id="widgetLabelTab"><img src="/<?php echo drupal_get_path('module', 'vsite_layout') . '/theme/images/widgetBarLabel.png'; ?>"></div>
<div id="websiteLabelTab"><img src="/<?php echo drupal_get_path('module', 'vsite_layout') . '/theme/images/websiteLayoutBarLabel.png'; ?>"></div>
<div class="<?php print $wgts_id; ?>-wrapper">

<ul id="widget-categories">
<?php
  foreach($s_tagged_output as $s_tag_name => $output){
  	$s_tag_id = strtolower( str_replace(' ', '_', $s_tag_name));
    echo "<li><a href=\"#{$s_tag_id}\">{$s_tag_name}</a></li>";
  }
?>
</ul>

<div class="tagged-list">
  <dl id="<?php print $wgts_id; ?>" class = "<?php print $wgts_class; ?>">
    <dt><?php print $region_name;?></dt>
	  <?php
	  echo implode(" ",$s_tagged_output);
  	?>
  </dl>
</div>
</div>





