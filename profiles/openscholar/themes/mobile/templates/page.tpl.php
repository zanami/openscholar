
<div id="page-wrapper"><div id="page">
  <div id="header-wrapper"><div class="container clearfix">
    <header id="header" class="clearfix">

      <div id="branding" class="clearfix">
        <?php if ($site_logo): ?>
          <div id="logo"><?php print $site_logo; ?></div>
        <?php endif; ?>

        <?php if ($site_name || $site_slogan): ?>
          <hgroup<?php if (!$site_slogan && $hide_site_name): ?> class="<?php print $visibility; ?>"<?php endif; ?>>
            <?php if ($site_name): ?>
              <h1 id="site-name"<?php if ($hide_site_name): ?> class="<?php print $visibility; ?>"<?php endif; ?>><?php print $site_name; ?></h1>
            <?php endif; ?>
            <?php if ($site_slogan): ?>
              <h2 id="site-slogan"><?php print $site_slogan; ?></h2>
            <?php endif; ?>
          </hgroup>
        <?php endif; ?>
      </div>

      <?php print render($page['header']); ?>

    </header>
  </div></div>

  <?php if ($page['menu_bar']): ?>
    <div id="nav-wrapper"><div class="container clearfix">
      <?php print render($page['menu_bar']); ?>
    </div></div>
  <?php endif; ?>

 

  <?php if ($messages || $page['help']): ?>
    <div id="messages-help-wrapper"><div class="container clearfix">
      <?php print $messages; ?>
      <?php print render($page['help']); ?>
    </div></div>
  <?php endif; ?>

  <div id="content-wrapper"><div class="container">
    <div id="columns"><div class="columns-inner clearfix">
      <div id="content-column"><div class="content-inner">

          <<?php print $tag; ?> id="main-content">

          <?php print render($title_prefix); ?>
          <?php if ($title || $primary_local_tasks || $secondary_local_tasks || $action_links = render($action_links)): ?>
            <header id="main-content-header">
              <?php if (!$is_front && $title): ?>
                <h1 id="page-title"><?php print $title; ?></h1>
              <?php endif; ?>

              <?php if ($primary_local_tasks || $secondary_local_tasks || $action_links): ?>
                <div id="tasks">
                  <?php if ($primary_local_tasks): ?>
                    <ul class="tabs primary clearfix"><?php print render($primary_local_tasks); ?></ul>
                  <?php endif; ?>
                  <?php if ($secondary_local_tasks): ?>
                    <ul class="tabs secondary clearfix"><?php print render($secondary_local_tasks); ?></ul>
                  <?php endif; ?>
                  <?php if ($action_links = render($action_links)): ?>
                    <ul class="action-links clearfix"><?php print $action_links; ?></ul>
                  <?php endif; ?>
                </div>
              <?php endif; ?>
            </header>
          <?php endif; ?>
          <?php print render($title_suffix); ?>

          <?php if ($content = render($page['content'])): ?>
            <div id="content">
              <?php print $content; ?>
            </div>
          <?php endif; ?>


      </div></div>

    </div></div>
  </div></div>



  <?php if ($page['footer']): ?>
    <div id="footer-wrapper"><div class="container clearfix">
      <footer class="clearfix">
        <?php print render($page['footer']); ?>
      </footer>
   </div></div>
  <?php endif; ?>

</div></div>
