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

; 3 col
regions[three_col_top]    = 3x33 Gpanel top
regions[three_col_first]  = 3x33 Gpanel left
regions[three_col_second] = 3x33 Gpanel center
regions[three_col_third]  = 3x33 Gpanel right
regions[three_col_bottom] = 3x33 Gpanel bottom

 */
?>
<!-- Three column 3x33 Gpanel -->
<?php if (
  $page['three_col_top'] ||
  $page['three_col_first'] ||
  $page['three_col_second'] ||
  $page['three_col_third'] ||
  $page['three_col_bottom']
  ): ?>
  <div class="at-panel gpanel panel-display three-col clearfix">
    <?php print render($page['three_col_top']); ?>
    <?php print render($page['three_col_first']); ?>
    <?php print render($page['three_col_second']); ?>
    <?php print render($page['three_col_third']); ?>
    <?php print render($page['three_col_bottom']); ?>
  </div>
<?php endif; ?>
