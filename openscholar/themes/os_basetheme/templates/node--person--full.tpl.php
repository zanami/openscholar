<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php print render($title_prefix); ?>
  <?php print render($title_suffix); ?>

  <div class="content-full"<?php print $content_attributes; ?>>
    <?php
      print render($content['field_professional_title']);
      print render($content['field_person_photo']);
      print render($content['pic_bio']);
      print render($content);
    ?>
  </div>
</div>
