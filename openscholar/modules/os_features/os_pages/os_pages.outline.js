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

  toggleChildren(input.parents('tr').find('.book-mlid').val());
  return false;
}

function toggleChildren(mlid) {
  $('.book-plid[value='+mlid+']').each(function () {
    var hidden = $(this).parents('tr').find('.visibility-toggle-link + input').get(0);
    hidden.value = Math.abs(hidden.value-1);
    $(this).parents('tr').find('.visibility-toggle-link').html(parseInt(hidden.value)?Drupal.t('Hide'):Drupal.t('Show'));
    toggleChildren($(this).parent().find('.book-mlid').val());
  });
}
})(jQuery)