Feature:
  Testing the private site access permissions.

  @api
  Scenario: Check the private site access.
    Given I go to "obama"
     Then I should see "This site is private, you must be logged in and be a site member to view."

  @api
  Scenario: Checking the access for the blog entry.
    Given I visit "obama/blog"
     When I should see "This site is private, you must be logged in and be a site member to view."
      And I fill in "Username" with "michelle"
      And I fill in "Password" with "FoureMoreYears"
      And I press "Log in"
      And I visit "obama/blog"
     Then I should see "Me and michelle obama"
