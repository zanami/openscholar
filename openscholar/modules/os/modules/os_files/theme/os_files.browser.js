/**
 * 
 */

(function ($, undefined) {
  Drupal.behaviors.osFilesMediaBrowser = {
    attach: function (ctx, s) {
      if ($('.media-item', ctx).length) {
        Drupal.ajax.prototype.commands.clickOn = function (ajax, response, settings) {
          $(response.target).bind('click', Drupal.media.browser.views.click).click();
        }
      
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
      }
        
      // prevents views filter form from submitting when Enter is pressed
      $('.ctools-auto-submit-full-form').submit(function (e) {e.preventDefault();});
      
      if ('hasClass' in ctx && ctx.hasClass('ctools-auto-submit-full-form')) {
        // set the focus to the search field 
        var $val = $('.views-exposed-form .views-exposed-widget input[type="text"]').focus().each(function () {
          // and move the cursor to the end of the text
          this.selectionStart = this.selectionEnd = this.value.length;
        });
      }
    }
  };
})(jQuery, undefined);