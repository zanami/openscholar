/**
 * Adds a new button for uploading and submits the form automatically
 */
Drupal.behaviors.os_upload_form = {
  attach: function (ctx) {
    var $ = jQuery,
        $input = $('<label for="edit-upload-upload" class="file-select form-submit">Upload</label>'),
        $file_select = $('#edit-upload input[type="file"]', ctx);
    $file_select.before($input).click(function() {
      $(this).addClass('clicked');
    });
    $input.bind('mousedown', function (e) {$input.addClass('focus');})
          .bind('mouseup', function(e) {$input.removeClass('focus');})
          .bind('click', function(e) {
        	if(!$file_select.hasClass('clicked')){
        	  $file_select.click();
            }
          });
    
    function changeHandler (e) {
      if (!('result' in e) || e.result) {
        $('#file-entity-add-upload .form-actions #edit-next', ctx).click();
      }
      $(this).removeClass('clicked');
    }
    
    $file_select.change(changeHandler).bind('propertychange', changeHandler);
  }
};
