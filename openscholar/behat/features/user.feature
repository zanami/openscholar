Feature: User page access
  In order to access exclusive personalized content
  As a site visitor
  I need to be be shown my user profile only when logged in.

  @api
  Scenario: User pages are inaccessible to anonymous users.
    Given I am not logged in
     When I visit "/users/john"
     Then I should see "Access Denied"

  Scenario: User pages are accessible to the logged in user.
    Given I am logged in as a user with the "vsite user" role
     When I visit "/user"
     Then I should see "View"
      And I should see "Edit"