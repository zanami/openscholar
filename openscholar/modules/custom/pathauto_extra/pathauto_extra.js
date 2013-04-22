/**
 * detects an illegal slash at the beginning of url path aliases.
 */
// Using the closure to map jQuery to $.
(function ($) {

    // Store our function as a property of Drupal.behaviors.
    Drupal.behaviors.myModuleSecureLink = {
        attach: function (context, settings) {
            // Find all the secure links inside context that do not have our processed
            // class.
            $(document).ready(function() {
                //remove front slash, show message
                $('#edit-path-alias').blur(function() {
                    var path = $('#edit-path-alias').attr('value');

                    //strip leading slash
                    if (path.indexOf('/') == 0) {
                        path = path.replace(/^\/+/, '');
                        pathauto_extra_warning('URL Path alias cannot begin with a slash.');
                    }

                    //strip trailing slash
                    if (path.substr(-1) == '/') {
                        path = path.replace(/\/+$/, '');
                        pathauto_extra_warning('URL Path alias cannot end with a slash.');
                    }

                    $('#edit-path-alias').attr('value', path);
                });

                //remove message, if displayed
                $('#edit-path-alias').focus(function() {
                    $('div.pathauto_extra-warning').remove();
                });

                //add listener to URL path settings.  fetch new path via ajax
                $('#edit-title').change(function() {
                    if (Drupal.settings.pathauto_extra && Drupal.settings.pathauto_extra.make_alias) {
                        // Prepares the ajax callback URL to query.
                        var href = href = 'http://' + document.location.host + '/pathauto_extra/alias_js';
                        // Prepares the URL parameters to call for this node.
                        var data = Drupal.settings.pathauto_extra;
                        // Cleans the title value stored in our URL params.
                        data.title = $.trim($('#edit-title').attr('value'));

                        if (data.title.length > 0 && $('#edit-path-pathauto').attr('checked')) {
                            // Adds autocomplete-style default Drupal throbber.
                            $('.form-item-title .description').html('<strong>Link URL:</strong> <span class="ajax-progress"><span class="throbber"></span></span>');
                            // Fetches the generated alias from the menu callback.
                            $.getJSON(href, data, function(json) {
                                if (json.status) {
                                    $('#edit-path-alias').attr('value', json.data);
                                    // @todo add base_url
                                    var base_url = Drupal.settings.pathauto_extra.prefix;
                                    var description = '<strong>Link URL:</strong> ' + base_url + json.data + ' <a id="pathauto-extra-edit-path" href="#path[pathauto]">edit</a>';

                                    $('.form-item-title .description').html(description);
                                    $('.form-item-title .description a').click(function() {
                                        $('html, body', context).animate({ scrollTop: $('.vertical-tabs').offset().top });
                                        $('.vertical-tab-button a:contains("URL path settings")', context).click();
                                        // @todo toggle click trigger to enable input
                                        $('#edit-path-pathauto').click().attr('checked', false).attr('value', 0);
                                        $('.form-item-path-alias').removeClass('form-disabled');
                                        $('#edit-path-alias').removeAttr("disabled").focus().select();
                                    });
                                }
                            });
                        }
                    }
                });

                function pathauto_extra_warning(str) {
                    $('#edit-path-alias + div.description').after('<div class="description pathauto_extra-warning">' + str + '</div>');
                    $('#edit-path-alias').change(); //.change() updates vtab preview
                }
            });
        }
    };

}(jQuery));

