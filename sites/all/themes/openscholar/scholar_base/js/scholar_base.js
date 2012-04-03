$(document).ready(function() {
  $('a.show_hidden_email').each(function(){
	  $(this).attr('href','mailto:'+$(this).attr('email_user')+'@'+$(this).attr('email_domain')).html('email');
  });
});