/**
 * @file
 * Override Google feed's module Drupal.behaviors.googleFeedAPI().
 */

(function ($) {

  /**
   * This script reads through feed settings and fetches feed data using the Google FeedAPI
   */
  Drupal.behaviors.googleFeedAPI = {
    attach: function (context, settings) {
      if (typeof google == 'undefined'){
        return;
      }

      //Loop through the feeds that are on this page
      $.each(settings.googleFeedAPI, function(div_id, feed_setting) {

        //Run the feed processing only once per feed
        $('div#'+div_id, context).once('googleFeedAPI', function () {
          //Load Feed
          var container = $(this);
          var feed = new google.feeds.Feed(feed_setting.url);
          feed.setNumEntries(feed_setting.num_feeds);
          feed.load(function(result) {
            if (!result.error) {
              for (var i = 0; i < result.feed.entries.length; i++) {
                var entry = result.feed.entries[i];

                var date = "";
                if (typeof entry.publishedDate != 'undefined' && entry.publishedDate != ''){
                  //@todo find a good way to do FuzzyTime in js
                  date = googleFeedAPIFuzzyDate(entry.publishedDate);
                  if (typeof date == 'undefined'){
                    date = "";
                  }
                  else {
                    date = "<div class='date'>" + date + "</div>";
                  }
                }

                var content = entry.content;

                if (feed_setting.content_summary && typeof entry.contentSnippet != 'undefined'){
                  content = entry.contentSnippet;
                }

                // Don't show the date in case of a title view.
                var feed_markup = "<div class='feed_item'>" +

                  "<a class='title' href='" + entry.link + "'>" + entry.title + "</a>";

                // If teaser view is used, add the date and the feed content.
                if (feed_setting.show_content){
                  feed_markup = feed_markup + date + "<span class='description'>" +
                    content +
                    "<span/>";
                }

                feed_markup = feed_markup + "</div>";

                var div = $(feed_markup);
                container.append(div);
              }

            }
          });
        });
      });
    }
  };

})(jQuery);
