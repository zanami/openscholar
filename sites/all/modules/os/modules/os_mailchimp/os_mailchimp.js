/**
 * @file os_mailchimp.js
 * 
 * attaches js for form processing for mailchimp subscriptions
 * this will probably be replaced by drupal behaviors later
 */

(function ($) {

Drupal.behaviors.os_mailchimp_modal = function() {
  $('.mailchimp-modal').click(function(e) {
    var url = $(this).attr('href');
    url += '?delta=' + Drupal.settings.os_mailchimp.delta;
    var modalOptions = {
      url: url,
      autoFit: true,
      onSubmit: Drupal.CTools.AJAX.respond,
      delta: Drupal.settings.os_mailchimp.delta,
    };  
    Drupal.modalFrame.open(modalOptions);
  
    e.preventDefault();
  });
 
}
})(jQuery);