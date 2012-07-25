/**
 * 
 */
Drupal.behaviors.osPublications = {
  attach: function (ctx) {
    // change the author category when the role is changed 
    var $ = jQuery,
        $hidden = $('#edit-biblio-contributors input[type="hidden"]');
    $hidden.each(function (i) {
      var $this = $(this),
          $select = $this.parents('tr').find('select');
      $select.change(function () {
        $this.val($(this).val());
      });
    });
  }
};