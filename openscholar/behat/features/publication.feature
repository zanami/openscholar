Feature:
  Testing the publication tab.

  @api
  Scenario: Test the Publication tab
    Given I visit "john"
     When I click "Publications"
      And I click "The Little Prince"
     Then I should see "Anon. The Little Prince. United States; 1943."
