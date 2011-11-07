$(document).ready(function() {

    $('.front #header-main .box-os_boxes_simple div.boxes-box-content p:last').append('<a class="more" href="#">More</a>');

    var container = $("body.front #header-main #block-boxes-scholar_biocv_bio");
    container.find('div.node').prepend('<a class="more" href="#">CLOSE X</a>');
    container.find('div.node .content').prepend('<h3 class="cv-direct-download">Full CV: <a href="http://gking.harvard.edu/vitae/vitae.pdf">PDF</a></h3><div class="clear"></div>');


    $(".front #header-main a.more").click(function(event){
      container.find('a.more').toggle();
      if (container.is(".bio-open")) {
        container.removeClass("bio-open").animate({height:'0'},"1500");
      } else {
        container.addClass("bio-open").animate({height:'70%'},"1500");
      }
    });
    
    $(".front #pub_type_tabs").tabs({ fx: { opacity: 'toggle' } });
    
    $(".front #header-top .wrap").addClass('clearfix');
    
});
