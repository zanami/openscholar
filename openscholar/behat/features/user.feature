Feature: User functionality testing.

  @api
  Scenario: Verify that user pages are inaccessible to anonymous users.
    Given I am not logged in
     Then I should be redirected in the following <cases>:
  #  | Request                    | Response Code | Final URL   |
     | users/admin                | 403           | users/admin |

  @api
  Scenario: User pages are accessible to the logging in user.
    Given I am logging in as "john"
     When I visit "/user"
     Then I should see "View"
      And I should see "Edit"

  @api
  Scenario: Adding a user to a vsite.
    Given I am logging in as "john"
     When I visit "john/cp/users/add"
      And I fill in "User" with "michelle"
      And I press "Add users"
     Then I should see "michelle has been added to the group John."

  @api @nofar
  Scenario: Enable custom roles and permissions in a VSite.
    Given I am logging in as "john"
      And I visit "john/cp/users/permissions"
     When I click "Edit roles and permissions"
      And I click "form-submit"
      And I visit "john/cp/users/permissions"
     Then I should see "Save permissions"

  @api @nofar
  Scenario: Disable custom roles and permissions in a VSite.
    Given I am logging in as "john"
      And I visit "john/cp/users/roles"
     When I click "Restore default roles and permissions"
      And I click "form-submit"
      And I visit "john/cp/users/roles"
     Then I should not see "Add role"
