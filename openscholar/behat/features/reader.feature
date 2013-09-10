Feature:
  Testing the OS reader functionallity.

  @api
  Scenario: Test the Reader tab
    Given I visit "john"
     When I click "Reader"
     Then I should see "Engadget rss"

  @api
  Scenario: Test the OS reader feed importer.
    Given I am logging in as "admin"
      And I import feeds items
     When I visit "john/os-reader"
      And I import the feed item "JFK has ben murdered"
     Then I should dee the feed item "JFK has ben murdered" has imported
      And I should see "JFK has ben murdered"

  @api
  Scenario: Verify the anonymous user is being redirected to the feed item
            source page.
    Given I visit "john/news"
     When I click "JFK has ben murdered"
     Then I should see "Assassination of John F. Kennedy"
