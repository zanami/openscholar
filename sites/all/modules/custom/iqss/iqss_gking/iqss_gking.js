//Javascript to manipulate the Areas of Interest block on Garys Home Page
Drupal.behaviors.iqss_gking = function() {
		
  var areas = $('#content-main .box-vsite_taxonomy_fbt .boxes-box-content ul li ul li');
  if (areas.length) {
	//Setup the AOR block
    $('#content-main .box-vsite_taxonomy_fbt .boxes-box-content ul li ul li').hover(function() {
    	$('#content-main .box-vsite_taxonomy_fbt .boxes-box-content ul li ul li.active').removeClass('active');
    	$('#content-main .box-vsite_taxonomy_fbt .boxes-box-content ul li ul li div.description').fadeOut('fast');
    
    	$('#content-main .box-vsite_taxonomy_fbt .boxes-box-content .more').hide();
    	
    	$(this).addClass('active');
    	$(this).find('div.description').fadeIn('fast');
    });

    $('#content-main .box-vsite_taxonomy_fbt .boxes-box-content ul li.last ul li').filter(":first").each(function(index) {
  	  $(this).hover();
    });
  }
  
  //Scroll to the top of the page when you click the sort links
  iqss_gking_taxonomy_alter_sort_links();
};

function iqss_gking_taxonomy_alter_sort_links(){
	if(!$('body.page-taxonomy').length) return;
	
	$('a.term-admin-sort-link').click(function(){ window.scroll(0,0); })
}