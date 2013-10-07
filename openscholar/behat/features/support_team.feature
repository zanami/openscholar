Feature:
  Testing ability to subscribe as support team for privileged users,
  that creates an expirable membership.

  @api @current
  Scenario: Test subscribe for user with permission
    Given I am logging in as "bill"
     When I visit "obama"
      And I click "Support Obama"
     Then I should see "Are you sure you want to join the web site Obama"

  @api @current
  Scenario: Test expiring membership on cron, of an exisiting member
    Given I am logging in as "bill"
     When I visit "obama"
      And I execute cron
     Then I should not see "Support Obama"

  @api @current
  Scenario: Test expiring membership on cron, of an exsiting member
    Given I am logging in as "bill"
     When I visit "obama"
      And I set "vsite_support_expire" to "1 sec"
      And I execute cron
     Then I should see "Support Obama"

  @api @current
  Scenario: Test subscribe for user without permission
    Given I am logging in as "michelle"
     When I visit "obama"
     Then I should not see "Support Obama"
