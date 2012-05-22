/**
 *  Javascript for the Control Panel menu.
 *  
 *   Does two things:
 *   1. Changes the menu select when the user drags a row
 *   2. Removes the 'hidden' class when the user selects a new menu from the select.
 */
(function($) {

function changeSelect() {
  var $this = $(this.oldRowElement),
    $prev = $this.prevAll('.section-heading'),
    val = $prev.find('.menu-name').val(),
    select = $this.find('.menu-name');
  
  select.val(val);
}

function changeRegion() {
  // remove the hidden class
  $('input').filter(function (i) {
     return (this.value && this.value == self.value);
  }).parents('tr').removeClass('hidden');
  
  // move the field to the new region
}
  
Drupal.behaviors.cp_menu_form = {
  attach: function (ctx) {
    // remove the 'hidden' class when a menu is changed
    $('select.menu-name', ctx).change(changeRegion);
    
    var drag = Drupal.tableDrag['edit-menu-table'];
    drag.onDrop = changeSelect;
  }
};
})(jQuery);