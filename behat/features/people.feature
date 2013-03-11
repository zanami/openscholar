Feature:
  Testing the people tab.

  @api
  Scenario: Test the People tab
    Given I visit "john"
     When I click "People"
      And I click "John Fitzgerald Kennedy"
     Then I should see "often referred to by his initials JFK"

  @api
  Scenario: Verfity that "People" tab shows all nodes.
    Given I visit "john/people"
     Then I should see "John Fitzgerald Kennedy"
      And I should see "Norma Jeane Mortenson"

  @api
  Scenario: Verfity that "People" tab shows can filter nodes by term.
      Given I visit "john/people/science/air"
       Then I should see "John Fitzgerald Kennedy"
        And I should not see "Norma Jeane Mortenson"
