$(document).ready(function() {
  $('#navbar li a').hover(
      function () {
          $(this).stop().animate({backgroundColor:'#fa8b18'}, 300);
          }, function () {
          $(this).stop().animate({backgroundColor:'#db7002'}, 100);
      });

   $('#navbar h1 a').hover(
      function () {
          $(this).stop().animate({backgroundColor:'#000000'}, 300);
          }, function () {
          $(this).stop().animate({backgroundColor:'#636363'}, 100);
      });

    $('#block-vsite_menus-features ul.menu li a').hover(
      function () {
          $(this).stop().animate({backgroundColor:'#F58714'}, 300);
          }, function () {
          $(this).stop().animate({backgroundColor:'#ffffff'}, 100);
      });

    $('.front #header-main .box-os_boxes_simple div.boxes-box-content').append('<a class="more" href="#">More</a>');

    var container = $("body.front #header-main #block-boxes-scholar_biocv_bio");
    container.find('div.node').prepend('<a class="more" href="#">CLOSE X</a>');
    container.find('div.node .content').prepend('<h3 class="cv-direct-download">Full CV: <a href="http://gking.harvard.edu/vitae/vitae.pdf">PDF</a></h3><div class="clear"></div>');


    $(".front #header-main .box-os_boxes_simple a.more").click(function(event){
      container.find('a.more').toggle();
      if (container.is(".bio-open")) {
        container.removeClass("bio-open").animate({height:'0'},"1500");
      } else {
        container.addClass("bio-open").animate({height:'70%'},"1500");
      }
    });

  });
