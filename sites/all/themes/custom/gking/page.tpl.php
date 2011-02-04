<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>

<body class="<?php print $body_classes; ?>">
  <!--[if lte IE 6]><script src="/<?php print $directory;?>/js/ie6-warning/ie6-warning.js"></script> <script>window.onload=function(){e("/<?php print $directory;?>/js/ie6-warning/")}</script><![endif]-->
  <div id="wrapper">
    <div id="wrapper-inner">
      <div id="container">

      <?php if ($header_top || $header_main || $header_left || $header_right || $navbar) : ?>

      <div id="header">
        <?php if (!empty($header_top)): ?>
        <div id="header-top">
          <?php print $header_top; ?>
        </div><!-- /header-top -->
        <?php endif; ?>
        <?php if ($header_main || $header_left || $header_right) : ?>
        <div id="header-wrapper">
          <?php if (!empty($header_main)): ?>
            <div id="header-main" class="column">
              <?php print $header_main; ?>
            </div><!-- /header-main -->
          <?php endif; ?>

          <?php if (!empty($header_left)): ?>
            <div id="header-left" class="column">
              <?php print $header_left; ?>
            </div><!-- /header-left -->
          <?php endif; ?>

          <?php if (!empty($header_right)): ?>
          <div id="header-right" class="column">
            <?php print $header_right; ?>
          </div><!-- /header-right -->
            <?php endif; ?>
        </div><!-- /header wrapper -->
        <?php endif; ?>
        <?php if (!empty($navbar)): ?>
        <div id="navbar">
          <?php print $navbar; ?>
        </div><!-- /navbar -->
        <?php endif; ?>
      </div> <!-- /header -->
      <?php endif; ?>

      <div id="content-wrapper">
       <div id="content-wrapper-inner">
        <div id="content-main" class="column">
          <?php $path = drupal_get_path_alias($_GET['q']); ?>
          <?php if (!empty($content_top) || $path == 'classes'): ?>
            <div id="content-top">
              <?php print $content_top; ?>
              <?php
                if ($path == 'classes') {
                   $block = module_invoke('iqss_gking', 'block', 'view', 'student_materials');
                   //print '<div class="node">';
                   print '<h2 class="title">' . $block['subject'] . '</h2>';
                   print $block['content'];
                   //print '</div>';
                }
              ?>
            </div><!-- /content-top -->
            <?php endif; ?>

            <div id="content">
            <?php print $context_links;?>
            <?php if (!empty($title)): ?>
              <h2 class="title<?php if ($tabs) : print ' with-tabs'; endif;?>"><?php print $title; ?></h2>
             <?php endif; ?>
             <?php if (!empty($tabs)): ?>
              <div class="tabs"><?php print $tabs; ?></div>
              <?php endif; ?>
              <?php print $help; ?>
              <?php print $messages; ?>
              <?php print $content; ?>
            </div> <!-- /content -->

            <?php if (!empty($content_bottom)): ?>
              <div id="content-bottom">
                <?php print $content_bottom; ?>
              </div><!--/content-bottom-->
            <?php endif; ?>
            </div><!-- /content main -->

              <?php if (!empty($left)): ?>
          <div id="sidebar-first" class="sidebar column">
            <?php print $left; ?>
          </div> <!-- /sidebar-first -->
          <?php endif; ?>

          <?php if (!empty($right)): ?>
          <div id="sidebar-second" class="sidebar column">
            <?php print $right; ?>
          </div> <!-- /sidebar-second -->
          <?php endif; ?>
        </div> <!-- / content wrapper inner -->
      </div> <!-- / content wrapper -->
      <div id="footer">
        <div id="footer-inner">
          <?php
            $home_link =  l('Powered by OpenScholar','http://openscholar.harvard.edu', array('attributes' => array('class' => 'poweredby'),'html'=>TRUE));
            $login_link = theme('vsite_login_link',"Login",array('class' => 'footer-login'));
          ?>
          <p class="copy"><?php print $login_link;?> <?php if ($footer_message) { print $footer_message; } ?> <span id="powered-link"><?php print $home_link; ?></span></p><?php if(variable_get('openscholar_reportverinfo', 1)){ ?><img src="http://openscholar.harvard.edu/openscholar_lu/spacer.gif?<?php echo drupal_query_string_encode($openscholar_version_info) ?>" /><?php } ?>
          <?php if ($footer) : ?>
            <?php print $footer; ?>
          <?php endif; ?>
        </div><!-- /#footer-inner -->
      </div> <!-- /#footer -->
    </div> <!-- /container -->
    </div><!-- /wrapper-inner -->
  </div> <!-- /wrapper -->
  <div id="extradiv"></div>
  <?php if ($closure_region): ?>
    <div id="closure-blocks"><?php print $closure_region; ?></div>
  <?php endif; ?>
  <?php print $closure; ?>
</body>
</html>