/**
 * 
 */

(function ($, undefined) {
  Drupal.behaviors.osFilesMediaBrowser = {
    attach: function (ctx, s) {
      var $items = $('.media-item:not(.os-files-processed)', ctx),
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
       $(this).addClass('os-files-processed');
      });
    }
  };
})(jQuery, undefined);