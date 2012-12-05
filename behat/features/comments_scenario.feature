Feature:
  Testing the comment publishing for a blog entry.

  @api
  Scenario: Check that all of the apps are turned on
    Given I am logged in as a user with the "authenticated user" role
     When I visit "john/blog"
      And I publish a new blog entry
