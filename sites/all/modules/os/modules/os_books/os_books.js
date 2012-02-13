/**
 * Sets up a linkage between the links in a Table of Contents block 
 * or the book links in the content area and the actual contents of a book page
 */
(function($){
	var container, header, content = {}, active;
	
	Drupal.behaviors.os_book_linkage = function(ctx) {
		if (!$('body.node-type-book').length) return;
		var nid, title,
			blocks = $().not('*');
		
		if (ctx == document) {
			content = Drupal.settings.book_pages;
			container = $('#content');
			header = $('#content-main .title').not('.book-menu .title');
			active = $('#content-main .node').attr('id').replace('node-','');
		}
		
		// pages is a list of every page in the book
		// we parse this content and assign the contents to an nid in a js object
		// as we do this, we check for links that have the same title and assign
		// the nid as an attribute.
		for (nid in content) {
			title = content[nid].title;
			// add nid as attribute of links with same title
			$('.block a:contains("'+title+'"), .book-menu a:contains("'+title+'")').not('[data-nid]').each(function (index, elem) {
				elem.setAttribute('data-nid', nid);
				var parents = $(elem).parents('.block, .book-menu');
				blocks.add(parents);
			});
		}
		
		// attach click handler to blocks
		blocks.click(toc_click);
	};
	
	function toc_click(e) {
		var nid = e.target.getAttribute('data-nid');
		if (content[nid]) {
			e.preventDefault();
			$('.node', container).replaceWith(content[nid].content);
			// the 2nd .node is a different element from the first
			var node = $('.node', container);
			header.html(content[nid].title);
			
			// deal with the 'active' class
			$('a[data-nid="'+active+'"]').removeClass('active');
			$('a[data-nid="'+nid+'"]').addClass('active');
			
			active = nid;
			
			// change drupal settings
			Drupal.settings.getQ = "node/"+active;
			Drupal.settings.disqus.identifier = "node/"+active;	//TODO: Find a better way to do this
			
			Drupal.attachBehaviors(node[0]);
			node.hide().fadeIn();
			
			// start up jcarousels
			/*if (typeof Drupal.settings.jcarousel != 'undefined') {
				jQuery.each(Drupal.settings.jcarousel, function (selector, options) {
					$(selector+':visible').not('.has-jcarousel').addClass('has-jcarousel').removeClass('jcarousel-processed');
					Drupal.behaviors.jcarousel();
				});
			}*/
		}
	}
	
	// stick our behavior at the front so it runs before jcarousel
	/*
	var new_behaviors = {
		os_books_jcarousel: function () {
			if (typeof Drupal.settings.jcarousel != 'undefined') {
				jQuery.each(Drupal.settings.jcarousel, function(selector, options) {
					// prevents jcarousel from running on hidden elements
					$(selector).not(':visible').addClass('jcarousel-processed');
				});
			}
		}
	};
	Drupal.behaviors = $.extend(new_behaviors, Drupal.behaviors);*/

})(jQuery);