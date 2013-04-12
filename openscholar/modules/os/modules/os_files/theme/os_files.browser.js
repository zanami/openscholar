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
              url: s.basePath+('pathPrefix' in s?s.pathPrefix:'')+'file/'+fid+'/edit/nojs',
              event: 'click',
              wrapper: 'file-edit-section',
              method: 'html',
              progress: {type: 'throbber'}
            },
            ajax = new Drupal.ajax(base, this, settings);
       $(this).addClass('os-files-processed');
      });
      
      // Sometimes the ajax handler will just try to work with #undefined, which is wrong
      Drupal.ajax.prototype.beforeSubmit = function (values, element, options) {
        if (this.wrapper == '#undefined') {
          var form = $(this.selector).parents('form');
          if (form.length) {
            this.wrapper = '#'+form[0].id;
          }
        }
      };
      
      // makes the browser open the current tab by default when the form is reloaded
      $('#media-browser-tabset').tabs('option', 'select', function (event, ui) {
        if (ui.tab.hash) {
          window.location.hash = ui.tab.hash;
        }
      });
      
      // prevents views filter form from submitting when Enter is pressed
      $('.ctools-auto-submit-full-form').submit(function (e) {e.preventDefault();});
    }
  };
})(jQuery, undefined);