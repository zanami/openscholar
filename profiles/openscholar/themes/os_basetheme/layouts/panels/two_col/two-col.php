<?php
/**
 * Gpanels are drop in multi-column snippets for displaying blocks.
 * Most Gpanels are stacked, meaning they have top and bottom regions
 * by default, however you do not need to use them. You should always
 * use all the horizonal regions or you might experience layout issues.
 *
 * How to use:
 * 1. Copy and paste the code snippet into your page.tpl.php file.
 * 2. Copy and paste the region definitions to your themes .info file.
 * 3. Clear the cache (in Performance settings) to refresh the theme registry.

Region Deinitions:

; 2 col
regions[two_col_top]    = 2x50 Gpanel top
regions[two_col_first]  = 2x50 Gpanel left
regions[two_col_second] = 2x50 Gpanel right
regions[two_col_bottom] = 2x50 Gpanel bottom

 */
?>
<!-- Two column 2x50 -->
<?php if (
  $page['two_col_top'] ||
  $page['two_col_first'] ||
  $page['two_col_second'] ||
  $page['two_col_bottom']
  ): ?>
  <div class="at-panel gpanel panel-display two-col clearfix">
    <?php print render($page['two_col_top']); ?>
    <?php print render($page['two_col_first']); ?>
    <?php print render($page['two_col_second']); ?>
    <?php print render($page['two_col_bottom']); ?>
  </div>
<?php endif; ?>
