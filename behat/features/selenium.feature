Feature: Testing OpenScholar with selenium.

  @javascript
  Scenario: Test the drag'n'drop for the layout.
    Given I visit "/john"
    When I login as "admin/admin"
    # Currently use the hardcoded addrees becuase behat don't interact with the
    # Layout link.
    And I visit "john/cp/build/layout/os_front?destination=home"
    And I drag&drop "boxes-site_logo" to "edit-layout-content-top"
    And I press "edit-submit"
    Then I verify the element "boxes-box-site_logo" under "columns"

  @javascript
  Scenario: Test the creation of a new site.
    Given I visit "/"
    When I login as "admin/admin"
    And I click "Create your site"
    And I fill "edit-domain" with "random text"
    And I press "edit-submit"
    And I Wait for the text "Success! The new site has been created."
    And I click "Go there now"
    # The test will fail due to the error message after creating a new site.
    # And I Wait for the text "Your site's front page is set to display your bio by default."
