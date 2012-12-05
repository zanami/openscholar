Feature:
  Verify for the WSOD not the be shown when viewing a term page.

  @api
  Scenario: Verify that the user see the tagged node to the "Apple" term.
    Given I visit "john/software/term/10"
     Then I should see "IOS 6"
