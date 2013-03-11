Feature:
  Testing the activity stream

  @api
  Scenario: Check activity stream page
    Given I visit "/activity"
     Then I should see "John created Software Project: Windows 7"

