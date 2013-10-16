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

    }
  };
})(jQuery);
