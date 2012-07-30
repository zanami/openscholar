/**
 * 
 */
Drupal.behaviors.osPublications = {
  attach: function (ctx) {
    // change the author category to the role when the form is submitted
    var $ = jQuery;
    $('form').submit(function (i) {
      var hidden = $('#edit-biblio-contributors .biblio-contributor-category input[type="hidden"]').not('.autocomplete'),
          val = hidden.parents('tr').find('select').val();
      hidden.val(val);
    });
  }
};