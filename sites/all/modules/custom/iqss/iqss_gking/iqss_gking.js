//Javascript to manipulate the Areas of Interest block on Garys Home Page
Drupal.behaviors.iqss_gking = function() {
  //Init garys People page
  iqss_gking_research_group_tabify();		
  
  var areas = $('#content-main .box-vsite_taxonomy_fbt .boxes-box-content ul li ul li:not(.aor-processed)');
  if (areas.length) {
	//Setup the AOR block
    $('#content-main .box-vsite_taxonomy_fbt .boxes-box-content ul li ul li:not(.aor-processed)').hover(function() {
    	$('#content-main .box-vsite_taxonomy_fbt .boxes-box-content ul li ul li.active').removeClass('active');
    	$('#content-main .box-vsite_taxonomy_fbt .boxes-box-content ul li ul li div.description').fadeOut('fast');
    
    	$('#content-main .box-vsite_taxonomy_fbt .boxes-box-content .more').hide();
    	
    	$(this).addClass('active');
    	$(this).find('div.description').fadeIn('fast');
    });

    $('#content-main .box-vsite_taxonomy_fbt .boxes-box-content ul li.last ul li:not(.aor-processed)').filter(":first").each(function(index) {
  	  $(this).hover();
    });
    
    $('#content-main .box-vsite_taxonomy_fbt .boxes-box-content ul li ul li').addClass('aor-processed');
  }
  
  //Scroll to the top of the page when you click the sort links
  iqss_gking_taxonomy_alter_sort_links();
};

function iqss_gking_taxonomy_alter_sort_links(){
	if(!$('body.page-taxonomy').length) return;
	
	$('a.term-admin-sort-link').click(function(){ window.scroll(0,0); })
}

/**
 * For Garys People page
 */
function iqss_gking_research_group_tabify(){
  if(!$('body.page-iqss-gking-research-group').length) return;

  $("<ul id='research-group-tablist' class=\"ui-tabs-nav\"></ul>").insertBefore("div#content-main div.taxonomy-term-child:first");

  var terms = Drupal.settings.iqss_gking.research_group_terms;
  for (var i=0;i<terms.length;i++)
  {
    var url = "#iqss_gking_term_"+terms[i].tid;

    $('ul#research-group-tablist').append("<li class='research_group_tab'><a href='"+url+"'>"+terms[i].name+"</a></li>");
  }

  $("#content").tabs({ fx: { opacity: 'toggle' }, cache: true, load: function(event, ui) { Drupal.behaviors.CToolsDropdown(); } });
}