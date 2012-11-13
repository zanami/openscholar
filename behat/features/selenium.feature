Feature: Testing OpenScholar with selenium.

  @javascript
  Scenario: Test the Bio tab
    Given I visit "/john"
    When I login as "admin/admin"
    And I click "Links" with the class "ctools-dropdown-text-link"
    # Currently use the hardcoded addrees becuase behat don't interact with the
    # Layout link.
    And I visit "john/cp/build/layout/os_front?destination=home"
    And I drag’n’drop "boxes-site_logo" to "edit-layout-content-top"
    And I press "edit-submit"
    Then I verify the element "boxes-box-site_logo" under "columns"
