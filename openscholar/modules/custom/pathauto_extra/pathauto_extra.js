/**
 * Enhances node path alias entry with automatic preview on forms.
 */
(function ($) {
    // Stores behaviors as a property of Drupal.behaviors.
    Drupal.behaviors.alias_preview = {
        attach: function (context, settings) {
            $(document).ready(function () {
                // On node edit forms only, creates a static "Link URL" preview.
                // Finds any existing alias value,
                // then injects it into the title field description.
                if ($('.page-node-edit #edit-path-alias').length) {
                    var alias = $('#edit-path-alias').val();
                    if (alias.length) {
                        var base_url = Drupal.settings.pathauto_extra.prefix;
                        var description = '<strong>Link URL:</strong> ' + base_url + alias + ' <a id="pathauto-extra-edit-path" href="#path[pathauto]">edit</a>';
                        $('.form-item-title .description').html(description);
                    }
                }

                // On both node add and node edit forms, dynamically validates.
                // If a user enters a leading or trailing slash,
                // remove it when the input blurs, and display a message.
                $('#edit-path-alias').blur(function () {
                    var path = $('#edit-path-alias').attr('value');

                    // Strips leading slash
                    if (path.indexOf('/') == 0) {
                        path = path.replace(/^\/+/, '');
                        alias_preview_warning('URL Path alias cannot begin with a slash.');
                    }

                    // Strips trailing slash
                    if (path.substr(-1) == '/') {
                        path = path.replace(/\/+$/, '');
                        alias_preview_warning('URL Path alias cannot end with a slash.');
                    }

                    // Replaces the invalid value with the cleaned value.
                    $('#edit-path-alias').attr('value', path);
                });

                // Removes any previous warning messages when new value is input.
                $('#edit-path-alias').focus(function () {
                    $('div.pathauto_extra-warning').remove();
                });

                // On node add forms, prompt users to hit return and see preview.
                // For graceful degradation, the default behavior is a static
                // "no preview" message when javascript is not working.
                $('.page-node-add #edit-title').focus(function () {
                    if ($('#edit-title').val().length == 0) {
                        var description = '<strong>Link URL:</strong> <em>Enter text and press return to update preview.</em>';
                        $('.form-item-title .description').html(description);
                    }
                });

                // On node add forms, use AJAX callback to generate alias preview.
                $('.page-node-add #edit-title').change(function () {
                    // Verifies settings before continuing...
                    if (Drupal.settings.pathauto_extra && Drupal.settings.pathauto_extra.make_alias) {
                        // Prepares the ajax callback URL to query.
                        var href = href = 'http://' + document.location.host + '/pathauto_extra/alias_js';
                        // Prepares the URL parameters to call for this node.
                        var data = Drupal.settings.pathauto_extra;
                        // Cleans the title value stored in our URL params.
                        data.title = $.trim($('#edit-title').attr('value'));

                        // Only proceeds if there is a title value, and
                        // if the "Generate automatic alias" setting is checked.
                        if (data.title.length > 0 && $('#edit-path-pathauto').attr('checked')) {
                            // Adds autocomplete-style default Drupal throbber.
                            $('.form-item-title .description').html('<strong>Link URL:</strong> <span class="ajax-progress"><span class="throbber"></span></span>');
                            // Fetches the generated alias from the menu callback.
                            $.getJSON(href, data, function (json) {
                                // If we got a successful AJAX response...
                                if (json.status) {

                                    // Prepares the alias, optionally removing purl.
                                    var alias = json.data;
                                    var base_url = Drupal.settings.pathauto_extra.prefix;
                                    if (Drupal.settings.pathauto_extra.remove_purl) {
                                        var purl = Drupal.settings.pathauto_extra.purl;
                                        alias = alias.slice(purl.length + 1)
                                    }

                                    // Updates the existing pathauto alias field with the new value.
                                    $('#edit-path-alias').attr('value', alias);
                                    // Updates the title input field description to immediately show user.
                                    var description = '<strong>Link URL:</strong> ' + base_url + json.data + ' <a id="pathauto-extra-edit-path" href="#path[pathauto]">edit</a>';
                                    $('.form-item-title .description').html(description);

                                    // Provides a smooth animation when the user clicks "edit"
                                    // to bring the user to the custom alias field with proper settings checked.
                                    $('.form-item-title .description a').click(function () {
                                        $('html, body', context).animate({ scrollTop: $('.vertical-tabs').offset().top });
                                        $('.vertical-tab-button a:contains("URL path settings")', context).click();
                                        $('#edit-path-pathauto').click().attr('checked', false).attr('value', 0);
                                        $('.form-item-path-alias').removeClass('form-disabled');
                                        $('#edit-path-alias').removeAttr("disabled").focus().select();
                                    });
                                }
                            });
                        }
                    }
                });

                // Displays a small warning to the user that the original input
                // was invalid and has been replaced, explaining why.
                function alias_preview_warning(str) {
                    // Inserts the warning at the end of the existing description, on it's own line.
                    $('#edit-path-alias + div.description').after('<div class="description pathauto_extra-warning">' + str + '</div>');
                    // Triggers change() event in order to update vertical tabs preview text.
                    $('#edit-path-alias').change();
                }
            });
        }
    };
}(jQuery));
