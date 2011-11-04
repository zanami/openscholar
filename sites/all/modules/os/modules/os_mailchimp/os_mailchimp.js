/**
 * @file os_mailchimp.js
 * 
 * attaches js for form processing for mailchimp subscriptions
 * this will probably be replaced by drupal behaviors later
 */

$().ready( function() {
  init_mailchimp();
  alert('we have mailchimp js');
})

function init_mailchimp() {
  $('#mailchimp_sub').click( function() {
    
    $(this).hide();
    $('#mailchimp_form').show();
  });
  
  
}