<?php
/**
 *  template for theming a widget
 *  Variables:
 *  ----------
 *  $s_widget_key -> the widgets key
 *  $w -> the widget
 */
$s_class = (isset($w['hidden']) && $w['hidden'])? 'scholarlayout-item disabled':'scholarlayout-item';
?>

<dd class="<?php echo $s_class. $w['tags_class'] ?><?php if($w['overides']) {?> with-overrides<?php } ?><?php if ($w['tags']) {foreach ($w['tags'] as $tag){echo ' '.$tag;}} ?>" id="<?php print $s_widget_key; ?>"> <?php print $w['label']; ?>
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