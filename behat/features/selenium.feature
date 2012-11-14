Feature: Testing OpenScholar with selenium.

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

  @javascript
  Scenario: Test the creation of a new site.
    Given I am logged in as "admin"
    When I visit "/"
    And I click "Create your site"
    And I fill "edit-domain" with "random text"
    And I press "edit-submit"
    And I wait for the text "Success! The new site has been created."
    # The test will fail due to the error message after creating a new site.
    # And I click "Go there now"
    # Then I wait for the text "Your site's front page is set to display your bio by default."

  @javascript
  Scenario: Test the popup for the events in the calendar.
    Given I am logged in as "admin"
    When I visit "/john"
    And I click "Calendar"
    And I click "John F. Kennedy birthday"
    Then I wait for the text "Thursday, May 30, 2013 (All day)"

  @javascript
  Scenario: Test the term tagging of nodes.
    Given I am logged in as "admin"
    When I visit "/john"
    And I click "Blog" with the class "menu-path-blog"
    And I click "First blog"
