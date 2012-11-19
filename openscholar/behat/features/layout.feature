Feature: Testing OpenScholar layout.

  @javascript
  Scenario: Test the drag&drop for the layout.
    Given I am logged in as "admin"
    When I visit "/john"
    # Currently use the hardcoded addrees becuase behat don't interact with the
    # Layout link.
    And I visit "john/cp/build/layout/os_front?destination=home"
    And I drag&drop "boxes-site_logo" to "edit-layout-content-top"
    And I press "edit-submit"
    Then I verify the element "boxes-box-site_logo" under "columns"
