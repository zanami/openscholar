/**
 @file

 Use the Media Browser to select an image to crop
*/
(function ($) {

Drupal.behaviors.osFilesImageCropBrowser = {
  attach: function (ctx) {
    $('.media-button', ctx).click(mediaButtonClick);
  }
}

  function mediaButtonClick(e) {
    var settings = {
      types: ['image']
    };
    Drupal.media.popups.mediaBrowser(function (selected) {
      // construct the html fragment and put it in place
      console.log(selected);
      if (selected.length == 0) {
        // remove cropping html
      }
      else {
        var file = selected[0];
        console.log(e);
      }
    }, settings);
    return false;
  }

})(jQuery);