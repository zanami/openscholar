/**
 * Sets up a linkage between the links in a Table of Contents block 
 * or the book links in the content area and the actual contents of a book page
 */
(function($){
	var display, header, content = {}, active, perma;
	
	Drupal.behaviors.os_book_linkage = function() {
		if (!$('body.node-type-book').length) return;
		var pages = $('.book-page');
		display = $('#content p').not('.book-page p').parent();
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
	}
	
	function toc_click(e) {
		var nid = e.target.getAttribute('nid');
		if (content[nid]) {
			$('*:not(.book-menu, .book-menu *)', display).remove();
			display.append(content[nid].content);
			$('*:not(.book-menu,.book-menu *)', display).fadeIn();
			header.html(content[nid].title);
			perma.attr('href', e.target.getAttribute('href'));
			e.preventDefault();
			
			// change the admin contextual links
			var reg = new RegExp(active, "g");
			display.parents('.node').find('.ctools-dropdown-container a').each (function () {
				this.href = this.href.replace(reg, nid);
			});
		}
	}
})(jQuery);