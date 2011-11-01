/**
 * Sets up a linkage between the links in a Table of Contents block 
 * or the book links in the content area and the actual contents of a book page
 */
(function($){
	var display, header, content = {}, nids = {}, perma;
	
	Drupal.behaviors.os_book_linkage = function() {
		if (!$('body.node-type-book').length) return;
		var pages = $('.book-page');
		display = $('#content p').not('.book-page p').parent();
		var blocks = $().not('*');
		perma = $('#book-permalink');
		header = $('#content-main .title');
		
		pages.each(function(index, elem){
			// get the nid of the content
			var nid = parseInt(elem.id.replace('book-node-',''));
			
			// save it to the content object
			if (typeof content[nid] == 'undefined') {
				var $elem = $(elem),
					title = $elem.find('h1:first').hide().html();
				
				// add nid as attribute of links with same title
				$('.content a:contains("'+title+'")').each(function (index, elem) {
					elem.setAttribute('nid', nid);
					var parents = $(elem).parents('.content');
					blocks.add(parents);
				});
				content[nid] = {title: title, content: $elem.html()};
			}
			else {
				content[nid].content += $(elem).html();
			}
		});
		
		// attach click handler to blocks
		blocks.click(toc_click);
	}
	
	function toc_click(e) {
		var nid = e.target.getAttribute('nid');
		if (content[nid]) {
			display.hide().html(content[nid].content).fadeIn();
			header.html(content[nid].title);
			perma.attr('href', e.target.getAttribute('href'));
			e.preventDefault();
			
			// when the book links are in the content area, 
			// the nids can't be added to the link
			$('a', display).each(function () {
				if (!this.getAttribute('nid')) {
					for (var i in content) {
						if (content[i].title == (this.innerText || this.textContent)) {
							this.setAttribute('nid', i);
						}
					}
				}
			});
		}
	}
})(jQuery);