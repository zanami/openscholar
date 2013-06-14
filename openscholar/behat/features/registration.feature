Feature:
  Testing the registration module.

  @api
  Scenario: Limit the registration capacity to 1 and verify it for a normal user.
    Given I am logged in as "john"
     When I visit "john/event/halleys-comet"
      And I set the event capacity to "1"
      And I select "Myself" from "This registration is for"
      And I press "Save Registration"
      And I am not logged in
     When I am logged in as "michelle"
      And I visit "john/event/halleys-comet"
      And I should not see "You are registering: Myself"
     Then I delete "john" registration

  @api
  Scenario: Limit the registration capacity to 2 and verify it for a normal user.
    Given I am logged in as "john"
     When I visit "john/event/halleys-comet"
      And I set the event capacity to "2"
      And I select "Myself" from "This registration is for"
      And I press "Save Registration"
      And I am not logged in
     When I am logged in as "michelle"
      And I visit "john/event/halleys-comet"
      And I should see "You are registering: Myself"
     Then I delete "john" registration
