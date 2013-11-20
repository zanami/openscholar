<div class="top-wrapper clearfix">
  <?php print $os_logo; ?>
  <div class="tagline"><?php print $tagline; ?></div>
  <div class="first-wrapper">
    <?php print render($search_block); ?>
  </div>
  <div class="last-wrapper">
    <div class="btn-wrapper">
      <?php print $create_link; ?>
    </div>
  </div>
</div>

<div class="middle-wrapper">

</div>

<div class="bottom-wrapper">

</div>

<div id="message_welcome_message">
  <div class="toggle"><?php print $short_message; ?></div>
    <div id="welcome_wrapper" class="os-slider">
    <h3><?php print $welcome_message; ?></h3>
    <p><?php print $below_message; ?></p>
    <?php print $items; ?>
  </div> <!-- SLIDER -->
</div>
