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
     When I visit "john/os-reader"
      And I should print page
      And I import the feed item "JFK has ben murdered"
     Then I should dee the feed item "JFK has ben murdered" has imported
      And I should see "The president JFK has been murdered"
