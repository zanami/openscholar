<?php /* dpm(get_defined_vars());*/  ?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $node_classes; ?>">
  <div class="node-inner">
    <div class="os-links">
      <?php print $vsite_admin_links; ?>
    </div>
    <?php if (!$page): ?>
      <h3 class="title">
        <a href="<?php print $node_url; ?>" title="<?php print $title ?>"><?php print $title; ?></a>
      </h3>
    <?php endif; ?>
    <?php if ($unpublished): ?>
      <div class="unpublished"><?php print t('Unpublished'); ?></div>
    <?php endif; ?>
     <?php if ($page): ?>
     	 <?php if ($submitted): ?>
     <div class="submitted">
        <?php print $submitted; ?>
      </div>
 <?php endif; ?>
     	<div class="submitted-by">submitted by <span class="blue"><?php print $node->name ?></span>
     <?php if ($terms): ?>
        <?php print t(' in ') . $terms; ?>
     <?php endif; ?>
     </div><br><br>
     <?php endif; ?>
    <div class="content">
      <?php print $content; ?> 
      <?php if ($submitted): ?>
      	<?php if (!$page): ?>
      <div class="submitted">
        <?php print $submitted; ?>
      </div>
      <?php endif; ?>
     <?php endif; ?>
    </div>
    <?php if (!$page): ?>
    <div class="submitted-by">submitted by <span class="blue"><?php print $node->name ?></span><?php if ($terms): ?> <?php print t(' in ') . $terms; ?><?php endif; ?></div>
    <?php endif; ?>	
    <?php if ($links): ?>
      <div class="links links-inline">
        <?php print $links;?>
      </div>
    <?php endif;?>
  </div> <!-- /node-inner -->
</div> <!-- /node -->


