<?php
/**
 *  template for theming a widget
 *  Variables:
 *  ----------
 *  $s_widget_key -> the widgets key
 *  $w -> the widget
 */
$s_class = (isset($w['hidden']) && $w['hidden'])? 'scholarlayout-item disabled':'scholarlayout-item';

if($w['overides']) {
	$s_class .= " with-overrides";
}

if(is_array($w['tags']) && count($w['tags'])) {
  foreach ($w['tags'] as $s_tag_name) {
  	$s_class .= " ".strtolower( str_replace(' ', '_', $s_tag_name));
  }
}else{
	$s_class .= " misc";
}

//Support for ctools popups
ctools_include('ajax');
ctools_include('modal');
?>

<dd class="<?php echo $s_class ?>" id="<?php print $s_widget_key; ?>"> <?php print $w['label']; ?>
      <div class="close-this">Remove</div>
     <?php
     if($w['block_config_path']){
     	 $class = (strpos($w['delta'],"boxes_add__") === 0)?"add":"setting";
       print ctools_modal_text_button("Configure",$w['block_config_path']."/cp_layout","open the form to configure this block",$class);
     }
     if($w['overides']){
       ?>
       <span class="scholarlayout-item-settings">Appears here on all pages with <span class="toggle-exceptions">exceptions</span>
        <div class="layout-exceptions">
          <h4>Exceptions:</h4>
          <ul>
            <?php
            $removethis = array("scholar", "Scholar");
            print "<li>" . implode(' , </li><li>',$w['overides']) . "</li>";
           ?>
          </ul>
        </div>
       </span>
       <?php
     }
     ?>
</dd>