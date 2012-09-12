<?php
// $Id$

/**
 * @file
 * Iframe that displays remote rbuild docs from http://r.iq.harvard.edu
 */
?>
<div id="rbuild-docs">
  <?php if (!empty($path)): ?>
    <iframe name="res" scrolling="auto" src ="<?php print $path; ?>">
    </iframe>
  <?php endif; ?>
</div>
