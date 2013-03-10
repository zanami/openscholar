Feature:
  Testing the documents tab.

  @api
  Scenario: Test the Documents tab
    Given I visit "john"
     When I click "Documents"
     Then I should see "All about nodes"

  @api
  Scenario: Verfity that "classes" tab shows all nodes.
    Given I visit "john/documents"
     Then I should see "All about nodes"
      And I should see "All about terms"

  @api
  Scenario: Verfity that "classes" tab shows can filter nodes by term.
     Given I visit "john/documents/science/air"
      Then I should see "All about nodes"
       And I should not see "All about terms" under "content"
