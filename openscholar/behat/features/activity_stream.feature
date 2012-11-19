Feature:
  Testing the activity stream

  @api
  Scenario: Check activity stream page
    Given I am logged in as a user with the "authenticated user" role
    When I visit "/activity"
    Then I should see "John created Class Material: Who was JFK?"

