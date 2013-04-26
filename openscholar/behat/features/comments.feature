Feature:
  Testing the comment publishing for a blog entry.

  @api
  Scenario: Site member adds a private comment to an existing blog post
    Given I am logged in as a user with the "administrator" role
  #   And I am a member of the site
  #   And Comments are set to "Private comments" for node type "blog"
     When I visit "john/blog"
      And I click "First blog"
  #   And I add a comment "Lorem ipsum john doe" using the comment form
  #  Then I should see "Lorem ipsum john doe"
