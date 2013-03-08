/**
 * 
 */

(function ($, undefined) {
  Drupal.behaviors.osFilesMediaBrowser = {
    attach: function (ctx, s) {
      // do nothing on ajax that have no browser
      if ($(ctx).find('#media-browser-library-list').length == 0) return;
      
      var $items = $('#media-browser-library-list li .media-item', ctx),
          forms = {};
      
      $items.each(function () {
        var base = this.id,
            fid = this.getAttribute('data-fid'),
            settings = {
              url: s.basePath+'file/'+fid+'/edit/nojs',
              event: 'click',
              wrapper: 'file-edit-section',
              method: 'html',
              progress: {type: 'throbber'}
            },
            ajax = new Drupal.ajax(base, this, settings);
      });
    }
  };
})(jQuery, undefined);