Feature:
  Testing the bio tab.

  @api
  Scenario: Test the Bio tab
    Given I visit "john"
     When I click "Bio"
      And I click "John doe biography"
     Then I should see "Work in gizra inc."
