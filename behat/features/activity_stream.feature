Feature:
  Testing the activity stream

  @api
  Scenario: Check activity stream after creating a node
    Given I am logged in as a user with the "administrator" role
    And I visit "john"
    When I click "Blog"
    Then I should see "Create Blog entry"
    When I fill in "title" with "Dummy blog entry"
    And I fill in "edit-body-und-0-value" with "Dummy blog entry"
    Then I press "Save"
    When I visit "/activity"
    And I should see "Dummy blog entry"

  # Comment out for now.
  # @api
  # Scenario: Check activity stream after editing a node
    # Given I am logged in as a user with the "administrator" role
    # And I visit "john"
    # When I am on a "bio" page titled "John doe biography"
    # And I click "Edit"
    # And I fill in "title" with "John doe biography - edited"
    # Then I press "Save"
    # When I clear the cache
    # And I visit "/activity"
    # Then I should see "John doe biography - edited"
