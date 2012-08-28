<?php 
/**
 * @file
 * Custom teaser and full node display overrides for Presentation nodes.
 * 
 * @see /profiles/openscholar/modules/os_features/os_presentations
 */
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $node_classes; ?>">
  <div class="node-inner">
    <?php if (!$page): // begin teaser ?>
      <span class="title">
      	<a href="<?php print $node_url; ?>" title="<?php print $title ?>">
      	  <?php print $title; ?>
      	</a>
      	<?php if ($node->field_presentation_location[0]['value']): ?>
      		<?php print ', '; ?>
        <?php endif; ?>
      </span>
      <?php if ($node->field_presentation_location[0]['value']):?>
        at 
        <span class="location">
          <?php print $node->field_presentation_location[0]['value']; ?>
          <?php if ($node->field_presentation_date[0]['value']): ?>
            <?php print ', '; ?>
          <?php endif; ?>
        </span>
      <?php endif; ?>
      <?php if ($node->field_presentation_date[0]['value']): ?>
        <?php print $node->field_presentation_date[0]['view']; ?>
        <?php if ($node->field_presentation_file[0]['fid']): ?>
          <?php print ': '; ?>
        <?php endif; ?>
      <?php endif; ?>
      <?php if ($node->field_presentation_file[0]['fid']): ?>
        <?php foreach ($node->field_presentation_file as $file): ?>
          <?php print $file['view']; ?>
        <?php endforeach; ?>
     <?php endif; ?>
   <?php endif; // end teaser ?>  
   <?php if ($page): // begin full node ?>
      <?php if ($unpublished): ?>
      <div class="unpublished">
        <?php print t('Unpublished'); ?>
      </div>
      <?php endif; ?>
      <?php if ($terms): ?>
      <div class="terms terms-inline">
        <?php print t(' in ') . $terms; ?>
      </div>
      <?php endif; ?>
      <?php endif;?>
      <div class="content">
        <?php print $content; ?>
      </div>
      <?php if ($links): ?>
      <div class="links links-inline">
        <?php print $links;?>
      </div>
    <?php endif; // end full node ?>
  </div> <!-- /node-inner -->
</div> <!-- /node -->
