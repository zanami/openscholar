Feature:
  Testing the private site access permissions.

  @api
  Scenario: Check the private site access.
    Given I go to "obama"
     Then I should see "This site is private, you must be logged in and be a site member to view."
