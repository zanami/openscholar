/**
 * Sets up a linkage between the links in a Table of Contents block 
 * or the book links in the content area and the actual contents of a book page
 */
(function($){
	var display, header, content = {}, active, perma;
	
	Drupal.behaviors.os_book_linkage = function() {
		if (!$('body.node-type-book').length) return;
		var pages = $('.book-page');
		display = $('#content .book-page').parent();
		var blocks = $().not('*');
		perma = $('#book-permalink');
		header = $('#content-main .title').not('.book-menu .title');
		active = $('#content-main .node').attr('id').replace('node-','');
		
		pages.each(function(index, elem){
			// get the nid of the content
			var nid = parseInt(elem.id.replace('book-node-',''));
			
			// save it to the content object
			if (typeof content[nid] == 'undefined') {
				var $elem = $(elem),
					title = $elem.find('h1:first').remove().text();
				
				// add nid as attribute of links with same title
				$('.block a:contains("'+title+'"), .book-menu a:contains("'+title+'")').each(function (index, elem) {
					elem.setAttribute('nid', nid);
					var parents = $(elem).parents('.block, .book-menu');
					blocks.add(parents);
				});
				content[nid] = {title: title, content: $elem.hide().html()};
			}
			else {
				content[nid].content += $(elem).hide().html();
			}
		});
		
		// attach click handler to blocks
		blocks.click(toc_click);
	};
	
	function toc_click(e) {
		var nid = e.target.getAttribute('nid');
		if (content[nid]) {
			$('*:not(.book-menu, .book-menu *)', display).remove();
			display.append(content[nid].content);
			$('*:not(.book-menu,.book-menu *)', display).fadeIn();
			header.html(content[nid].title);
			perma.attr('href', e.target.getAttribute('href'));
			e.preventDefault();
			
			// deal with the 'active' class
			$('a[nid="'+active+'"]').removeClass('active');
			$('a[nid="'+nid+'"]').addClass('active');
			
			// change the admin contextual links
			var reg = new RegExp(active, "g");
			display.parents('.node').find('.ctools-dropdown-container a').each (function () {
				this.href = this.href.replace(reg, nid);
			});
			active = nid;
			
			// start up jcarousels
			if (typeof Drupal.settings.jcarousel != 'undefined') {
				jQuery.each(Drupal.settings.jcarousel, function (selector, options) {
					$(selector+':visible').not('.has-jcarousel').addClass('has-jcarousel').removeClass('jcarousel-processed');
					Drupal.behaviors.jcarousel();
				});
			}
		}
	}
	
	// stick our behavior at the front so it runs before jcarousel
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
	Drupal.behaviors = $.extend(new_behaviors, Drupal.behaviors);

})(jQuery);