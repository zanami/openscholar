Feature:
  Testing the link tab.

  @api
  Scenario: Test the Links tab
    Given I visit "john"
     When I click "Links"
     Then I should see "JFK wikipedia page"

  @api
  Scenario: Verfity that "classes" tab shows all nodes.
    Given I visit "john/links"
     Then I should see "JFK wikipedia page"
      And I should see "Marilyn Monroe"

  @api
  Scenario: Verfity that "classes" tab shows can filter nodes by term.
     Given I visit "john/links/science/air"
      Then I should see "JFK wikipedia page"
       And I should not see "Marilyn Monroe"
