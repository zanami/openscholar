<?php
/**
 *  template for theming a widget
 *  Variables:
 *  ----------
 *  $s_widget_key -> the widgets key
 *  $w -> the widget
 */
$s_class = (isset($w['hidden']) && $w['hidden'])? 'scholarlayout-item disabled':'scholarlayout-item';

vsite_layout_find_widget_plugin($w);

if ($w['plugin']) {
  $s_class .= ' '.$w['plugin'];
}

if($w['overides']) {
	$s_class .= " with-overrides";
}

if (isset($w['plugin'])) {
  $info = os_boxes_get_boxes_plugins($w['plugin']);
  $s_class .= ' '.implode(' ',$info['tags']);
  $info['block_config_path'] = "cp/osboxes/nojs/".$w['delta'];
}

//Support for ctools popups
ctools_include('ajax');
ctools_include('modal');

if($w['icon_path']){
	$dd_il_style = "style=\"background-image:url('{$w['icon_path']}');\"";
}
?>

<dd class="<?php echo strtolower($s_class); ?>" id="<?php print $s_widget_key; ?>" <?php $dd_il_style ?>> <?php print $w['label']; ?>
      <div class="close-this" title="Remove">Remove</div>
     <?php
     if($info['block_config_path']){
       print ctools_modal_text_button("Delete", $info['block_config_path']."/delete", "Delete widget");
     	 $class = (strpos($w['delta'],"boxes_add__") === 0)?"add":"setting";
       print ctools_modal_text_button("Configure",$info['block_config_path']."/configure/cp_layout","Configure widget",$class);
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