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
      And I import feed items
     When I visit "john/cp/content/import"
      And I import the feed item "JFK was murdered"
     Then I should see the feed item "JFK was murdered" was imported
      And I should see "JFK was murdered"

  @api
  Scenario: Verify the anonymous user is being redirected to the feed item
            source page.
    Given I visit "john/news"
     When I click "JFK was murdered"
     Then I should see "Assassination of John F. Kennedy"

  @api
  Scenario: Verify the imported news date is the original feed item date.
    Given I am logging in as "admin"
    And I import feed items
    And I visit "john/cp/content/import"
    And I import the feed item "JFK was murdered"
    When I visit "john/news"
    And I click "JFK was murdered"
    Then I should see "November 22, 1963"

