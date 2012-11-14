Feature: Testing OpenScholar with selenium.

  # @javascript
  # Scenario: Test the drag'n'drop for the layout.
    # Given I visit "/john"
    # When I login as "admin/admin"
    # And I click "Links" with the class "ctools-dropdown-text-link"
    # # Currently use the hardcoded addrees becuase behat don't interact with the
    # # Layout link.
    # And I visit "john/cp/build/layout/os_front?destination=home"
    # And I drag’n’drop "boxes-site_logo" to "edit-layout-content-top"
    # And I press "edit-submit"
    # Then I verify the element "boxes-box-site_logo" under "columns"

  @javascript
  Scenario: Test the creation of a new site.
    Given I visit "/"
    When I login as "admin/admin"
    And I click "Create your site"
    And I fill "edit-domain" with "random text"
    And I press "edit-submit"
    And I click "Go there now"
    And I sleep for "5"
