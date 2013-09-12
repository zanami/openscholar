Feature:
  Testing the OG moderation functionallity.

  @api @current
  Scenario: Verify the vsite admin can access to unpublished node.
    Given I am logging in as "john"
      And I visit "john/classes"
      And I should not see "Unpublished node"
     When I visit "john/classes/unpublished-node"
     Then I should see "Unpublished node"
      And I should see "This is a node made for tests"

  @api @current
  Scenario: Verify the vsite admin can access to unpublished node.
    Given I visit "john/classes"
    # Anonymous user get 403 - should not get that.
      And I should not see "Unpublished node"
     When I visit "john/classes/unpublished-node"
     Then I should get a "403" HTTP response

