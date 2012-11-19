Feature: Testing OpenScholar calendar page.

  @javascript @wip
  Scenario: Test the popup for the events in the calendar.
    Given I am logged in as "admin"
    When I visit "/john"
    And I click "Calendar"
    And I click "John F. Kennedy birthday"
    Then I should wait and see "Thursday, May 30, 2013 (All day)"
