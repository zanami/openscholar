Feature:
  Testing the OS reader functionallity.

  @api
  Scenario: Test the Reader tab
    Given I visit "john"
     When I click "Reader"
     Then I should see "Engadget rss"

  @api @current
  Scenario: Test the OS reader feed importer
    Given I am logging in as "admin"
     When I set feed item to import
      And I import feeds items
