/**
 * Modifies the content area on the Dataverse iframe page.
 * 
 * Adds a loading message, and removes it when the iframe loads.
 */
(function($){
  Drupal.behaviors.os_dataverse = {
    attach: function(context){
      // Adds "Loading Dataverse" message with throbber 
      $('#feature-dataverse').prepend('<h2 id="dvn_loading" class="title">Loading Dataverse<span class="views-throbbing">&nbsp</span></h2>');
      // Removes the "Loading Dataverse" message when iframe loads
      $('#feature-dataverse iframe').load(function () {
        $('#feature-dataverse #dvn_loading').remove();
      });
    }
  };
})(jQuery);