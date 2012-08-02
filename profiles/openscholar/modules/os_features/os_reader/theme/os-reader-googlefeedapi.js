
(function ($) {

/**
 * This script reads through feed settings and fetches feed data using the Google FeedAPI
 */
Drupal.behaviors.osReaderGoogleFeeds = {
  attach: function (context, settings) {
	  if (typeof google == 'undefined'){
		  return;
	  }
	  
	  //Loop through the feeds that are on this page
	  $.each(settings.osReaderGoogleFeeds, function(div_id, feed_setting) { 
		  
		  //Run the feed processing only once per feed
		  $('div#'+div_id, context).once('osReaderGoogleFeeds', function () {
			    //Load Feed
			    var container = $(this);
			    var feed = new google.feeds.Feed(feed_setting.url);
			    feed.setNumEntries(feed_setting.num_feeds);
		        feed.load(function(result) {
		          if (!result.error) {
		            for (var i = 0; i < result.feed.entries.length; i++) {
		           	  var entry = result.feed.entries[i];
		           	  
		           	  var date = "";
		           	  if(typeof entry.publishedDate != 'undefined'){
		           		//@todo find a good way to do FuzzyTime in js
		           		date = entry.publishedDate;
		           	  }
		           	  
		           	  var content = entry.content;
		           	
		           	  if(feed_setting.content_summary && typeof entry.contentSnippet != 'undefined'){
		           		content = entry.contentSnippet;
		           	  }
		           	  
		              var div = $("<div class='feed_item'>" +
		            		date +
                      		"<a class='title' href='" + entry.link + "'>" + entry.title + "</a>" +
                      		"<span class='description'>" +
                      		content + 
                      		"<span/>" +
                      		"</div>");
		   	          container.append(div);
		    	    }
		      	  
		          }
		        });
		  });
	  });
  }
};

})(jQuery);
