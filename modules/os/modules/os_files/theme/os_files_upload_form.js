/**
 * Adds a new button for uploading and submits the form automatically
 */
Drupal.behaviors.os_upload_form = {
  attach: function (ctx) {
    var $ = jQuery,
        $input = $('<input class="file-select form-submit" type="button" value="Upload">', ctx),
        $file_select = $('#edit-upload input[type="file"]', ctx);
    $input.click(function (e) {
      $file_select.click();
    });
    $file_select.before($input);
    $file_select.change(function (e) {
      if (!('result' in e) || e.result) {
        $('.form-actions #edit-submit', ctx).click();
      }
    });
  }
}