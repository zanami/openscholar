Feature:
  Testing the view access to the list of all files on the site.

  @api
  Scenario: Test view access for a vsite member.
    Given I am logging in as "michelle"
     When I visit "obama/file.json"
     Then I should get a "200" HTTP response

  @api
  Scenario: Test view access for a non-member of the vsite.
    Given I am logging in as "michelle"
     When I go to "washington/file.json"
     Then I should get a "403" HTTP response

  @api
  Scenario: Test view access for a anonymous user for a private group.
    Given I go to "washington/file.json"
     Then I should get a "403" HTTP response

  @api
  Scenario: Test view access for a anonymous user for a public group.
    Given I go to "john/file.json"
     Then I should get a "200" HTTP response
