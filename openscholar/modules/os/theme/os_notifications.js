/**
 * jQuery behaviors for platform notification feeds.
 */
(function ($) {
  Drupal.behaviors.os_notifications = {
    attach: function (context) {

      // Setup.
      var menuLinkSel = '#os-notifications-menu-link';
      $(menuLinkSel).attr('href', '#').parent('li').append($("<div id='os-notifications-list'/>"));
      var settings = Drupal.settings.os_notifications;
      var container = $('#os-notifications-list');
      if (typeof google == 'undefined') {
        return;
      }
      // @TODO: Add support for multiple feeds.
      var feed = new google.feeds.Feed(settings.url);
      var items = [];
      feed.setNumEntries(settings.max);
      feed.load(function (result) {
        if (!result.error) {
          for (var i = 0; i < result.feed.entries.length; i++) {
            var entry = result.feed.entries[i];
            var item = os_notifications_item(entry);
            items.push(item);
            console.log(item);
          }
        }
      });

      if (typeof hopscotch == 'undefined') {
        return;
      }
      var tour = {
        showPrevButton: true,
        scrollTopMargin: 100,
        id: "platform-notifications",
        steps: items
      };

      $('#os-notifications-menu-link').click(function() {
        hopscotch.startTour(tour);
        // Removes animation for each step.
        $('.hopscotch-bubble').removeClass('animated');
        // Allows us to target just this tour in CSS rules.
        $('.hopscotch-bubble').addClass('os-notifications');
      });
    }
  };

  /**
   * Renders a feed object into an HTML snippet.
   *
   * @param {object} entry
   * @returns {string} output
   */
  function os_notifications_item(entry) {
    var output = "<div class='feed_item'>";
    var date = "";
    if (typeof entry.publishedDate != 'undefined' && entry.publishedDate != '') {
      date = os_notifications_fuzzy_date(entry.publishedDate);
      if (typeof date == 'undefined') {
        date = "";
      } else {
        date = "<span class='date'>" + date + "</span>";
      }
    }
    output += date;
    output += "<span class='description'>" + content + "<span/>";

    //
    var content = entry.content;
    if (typeof entry.contentSnippet != 'undefined') {
      content = entry.contentSnippet;
    }
    output += content;
    output += "<br/><a class='title' target='_blank' href='" + entry.link + "'>Read more &raquo;</a></div>";

    var item = {
      title: entry.title,
      content:output,
      target: document.querySelector("#os-notifications-list"),
      placement: "bottom",
      yOffset: 20
    };
    return item;
  }

  /**
   * Takes an ISO time and returns a string with "time ago" version.
   *
   * @param time
   * @returns {string}
   */
  function os_notifications_fuzzy_date(time) {
    var date = new Date(time),
      diff = (((new Date()).getTime() - date.getTime()) / 1000),
      day_diff = Math.floor(diff / 86400);

    if (isNaN(day_diff) || day_diff < 0 || day_diff >= 31) {
      return;
    }

    return day_diff == 0 && (
      diff < 60 && "just now" ||
      diff < 120 && "1 minute ago" ||
      diff < 3600 && Math.floor(diff / 60) + " minutes ago" ||
      diff < 7200 && "1 hour ago" ||
      diff < 86400 && Math.floor(diff / 3600) + " hours ago") ||
      day_diff == 1 && "Yesterday" ||
      day_diff < 7 && day_diff + " days ago" ||
      day_diff < 31 && Math.ceil(day_diff / 7) + " weeks ago";
  }
})(jQuery);
