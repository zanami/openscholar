Feature:
  Testing the presentation tab.

  @api
  Scenario: Test the Persentation tab
    Given I visit "john"
     When I click "Presentations"
     Then I should see "JFK's biography"

  @api
  Scenario: Verfity that "classes" tab shows all nodes.
    Given I visit "john/presentations"
     Then I should see "JFK's biography"
      And I should see "JFK lorem"

  @api
  Scenario: Verfity that "classes" tab shows can filter nodes by term.
     Given I visit "john/presentations/science/air"
      Then I should see "JFK's biography"
       And I should not see "JFK lorem" under "content"
