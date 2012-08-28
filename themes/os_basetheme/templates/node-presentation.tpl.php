<?php 
/**
 * @file
 * Custom teaser and full node display overrides for Presentation nodes.
 * 
 * @see /themes/adaptivetheme/at_core/templates/node.tpl.php
 * @see /modules/os_features/os_presentations
 */

hide($content['comments']);
hide($content['links']);

?>
<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="node-inner">
  <?php print render($title_prefix); ?>
  
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
  <?php if ($page): // begin default adaptivetheme full page node tpl ?>
    <?php if(!empty($user_picture) || $display_submitted): ?>
      <footer<?php print $footer_attributes; ?>>
        <?php print $user_picture; ?>
        <p class="author-datetime">
          <?php print $submitted; ?>
        </p>
      </footer>
    <?php endif; ?>
      
    <div<?php print $content_attributes; ?>>
      <?php print render($content); ?>
    </div>
      
    <?php if ($links = render($content['links'])): ?>
      <nav<?php print $links_attributes; ?>>
        <?php print $links; ?>
      </nav>
    <?php endif; ?>
    
    <?php print render($content['comments']); ?>
    
    <?php print render($title_suffix); ?>
  <?php endif; ?>
  </div> <!-- /div.node-inner -->
</article>
