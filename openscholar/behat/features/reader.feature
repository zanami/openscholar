Feature:
  Testing the OS reader functionallity.

  @api
  Scenario: Test the Reader tab
    Given I visit "john"
     When I click "Reader"
     Then I should see "Engadget rss"

  @api
  Scenario: Test the OS reader feed importer
    Given I am logging in as "admin"
      And I import feeds items
     When I visit "john/os-reader"
      And I import the feed item "JFK has ben murdered"
     Then I should dee the feed item "JFK has ben murdered" has imported
      And I should see "From Wikipedia, the free encyclopedia"
      And I should see "Assassination of John F. Kennedy"
