Feature:
  Testing scenarios for cache invalidate.

  @api
  Scenario: Verify the cache is invalidate.
    Given I am logged in as a user with the "administrator" role
     When I set the variable "views_og_cache_invalidate_node" to "1"
      And I visit "john/people"
      And I click "Add Person"
      And I fill in "First Name" with "Foo"
      And I fill in "Last Name" with "bar"
      And I press "Save"
      And I visit "john/people"
     Then I should see "Foo Bar"

  @api
  Scenario: Verify the cache is not invalidate.
    Given I am logged in as a user with the "administrator" role
     When I set the variable "views_og_cache_invalidate_node" to "0"
      And I visit "john/people"
      And I click "Foo bar"
      And I click "Delete"
      And I press "Delete"
      And I visit "john/people"
      And I should see "Foo Bar"
      And I invalidate cache
      And I visit "john/people"
     Then I should not see "Foo Bar"

