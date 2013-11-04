Feature:
  Test the redirect of the "cancel" button on node forms.

  @api
  Scenario: Test redirect when user is in some app path (i.e. blog).
    Given I am logging in as "john"
      And I visit "john/blog"
      And I edit the node "first blog"
      And I click "Cancel"
     Then I should be on "john/blog"
