/**
 * Handles toggling of the visibility parameter
 */
(function ($) {
Drupal.behaviors.osPagesOutline = {
  attach: function (ctx) {
    $('.visibility-toggle-link', ctx).click(toggleVisibility);
  }
}

function toggleVisibility(e) {
  var input = $(e.target).parent().find('input');

  input.val(Math.abs(input.val()-1));
  if (parseInt(input.val())) {
    e.target.innerHTML = Drupal.t('Hide');
  }
  else {
    e.target.innerHTML = Drupal.t('Show');
  }
  return false;
}
})(jQuery)