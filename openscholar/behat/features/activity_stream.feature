Feature:
  Testing the activity stream

  @api
  Scenario: Check activity stream page
    Given I debug the page "/activity"
     Then I should see "John created Class Material: Who was JFK?"

