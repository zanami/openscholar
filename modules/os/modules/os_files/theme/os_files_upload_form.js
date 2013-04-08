/**
 * Adds a new button for uploading and submits the form automatically
 */
Drupal.behaviors.os_upload_form = {
  attach: function (ctx) {
    var $ = jQuery,
        $input = $('<label for="edit-upload-upload" class="file-select form-submit">Upload</label>'),
        $help = $('<div class="form-help"></div>')
        $file_select = $('#edit-upload input[type="file"]', ctx);
    $file_select.before($input);
    $input.bind('mousedown', function (e) {$input.addClass('focus');})
          .bind('mouseup', function(e) {$input.removeClass('focus');});
     
    $('.form-item-upload label[for="edit-upload"]', ctx).after($help);
    
    function changeHandler (e) {
      if (!('result' in e) || e.result) {
        $('#file-entity-add-upload .form-actions #edit-next', ctx).click();
      }
    }
    
    $file_select.change(changeHandler).bind('propertychange', changeHandler);
  }
};