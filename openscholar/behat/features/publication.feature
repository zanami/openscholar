Feature:
  Testing the publication tab and application.

  @api
  Scenario: Test the Publication tab
    Given I visit "john"
     When I click "Publications"
      And I click "The Little Prince"
     Then I should see "Anon. The Little Prince. United States; 1943."


  @api
  Scenario: Check the setting for the publication settings.
    Given I am logged in as a user with the "administrator" role
     When I visit "john/cp/build/features/os_publications"
      And I fill in "edit-biblio-rowsperpage" with "1"
      And I press "edit-submit"
      And I create a new publication
      And I visit "john/publications"
     Then I should see "Next"
