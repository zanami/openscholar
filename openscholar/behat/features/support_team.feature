Feature:
  Testing ability to subscribe as support team.

  @api @current
  Scenario: Test subscribe for user with permission
    Given I am logging in as "bill"
    When I visit "obama"
    And I click "Support Obama"
    Then I should see "Are you sure you want to join the web site Obama"

  @api @current
  Scenario: Test subscribe for user without permission
    Given I am logging in as "michelle"
    When I visit "obama"
    Then I should not see "Support Obama"
