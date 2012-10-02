/**
 * Custom jQuery effects for gking theme.
 */
(function ($) {
  Drupal.behaviors.gking = {
    attach: function (context) {

      /**
       * Prepares Bio overlay for front page header.
       */
      // Bio set to display: none by default, for browsers without js.
      var container = $(".front .region-header-second .block-boxes-os_boxes_bio");
      if (container.length) {
        container.toggle();
      
        // Adds necessary more/close links to show/hide bio node content.
        $('.front .region-header-second .block-boxes-os_boxes_html .boxes-box-content p:last')
          .append('<a class="more" href="#">More</a>');
        container.find('.node')
          .prepend('<a class="more" href="#">CLOSE X</a>');
        container.find('.node .node-content')
          .prepend('<h3 class="cv-direct-download">Full CV: <a href="http://gking.harvard.edu/vitae/vitae.pdf">PDF</a></h3><div class="clear"></div>');

        // Both the "More" and "Close X" links trigger this animate event.
        $(".front .region-header-second a.more").click(function (event) {
          if (container.hasClass("bio-open")) {
            container.removeClass("bio-open").animate({height: '0'}, "1500");
          } else {
            container.addClass("bio-open").animate({height: '70%'}, "1500");
          }
        });
      }
  
      /**
       * Prepares the People page with tabs.
       */
      if ($('.page-research-group').length) {
    	/*
        $("<ul id='research-group-tablist' class=\"ui-tabs-nav\"></ul>")
          .insertBefore("#content-main .taxonomy-term-child:first");
        var terms = Drupal.settings.gking.research_group_terms;
        for (var i = 0; i < terms.length; i++) {
          var url = "#iqss_gking_term_" + terms[i].tid;
          $('ul#research-group-tablist')
            .append("<li class='research_group_tab'><a href='" + url + "'>" + terms[i].name + "</a></li>");
        }*/
    	
        $("#people-tabs").tabs({fx: {opacity: 'toggle'}, cache: true, load: function (event, ui) { Drupal.behaviors.CToolsDropdown(); }});
      }
      
      /**
       * Prepares the "Areas of Research" front page taxonomy widget.
       */
      // DAN'S HOVER TO SHOW AOR DESCR - IT ONLY 1/2 WORKS (LACKS ABILITY TO SHOW DESCRIPTION WHEN MOUSE OFF OF A TERM)
//      $("ul.termchild-Methods a, ul.termchild-Applications a").hover(function(){
//    	$(this).removeClass("active");
//        $(this).toggleClass("line");
//        $(this).next("div.description").toggle();
//      });
      // Original method
      var $sel = '#content-column .block-boxes-os_taxonomy_fbt .boxes-box-content';
      var areas = $($sel + ' ul li ul li:not(.aor-processed)');
      if (areas.length) {
        // Setup the AOR block
        $($sel + ' ul li ul li:not(.aor-processed)').hover(function () {
          $($sel + ' ul li ul li.active')
            .removeClass('active');
          $($sel + ' ul li ul li div.description')
            .fadeOut('fast');
          $($sel + ' .more')
            .hide();
          $(this)
            .addClass('active')
            .find('div.description')
            .fadeIn('fast');
        });
        $($sel + ' ul li.last ul li:not(.aor-processed)')
          .filter(":first")
          .each(function (index) {
            $(this).hover();
        });
        $('#content-column .block-boxes-os_taxonomy_fbt .boxes-box-content ul li ul li')
          .addClass('aor-processed');
      }
      
      /**
       * Scrolls to the top of the page when you click the sort links
       */
//      if ($('body.page-taxonomy').length) {
//        $('a.term-admin-sort-link')
//          .click(function () {
//          window.scroll(0, 0);
//        });
//      }
    }
  };
})(jQuery);