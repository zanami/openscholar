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
  Scenario: Test the filters on OS feed import page.
    Given I am logging in as "admin"
    And I import feed items
    And I visit "john/cp/content/import?feed_by_text=president"
    And I should see "JFK was murdered"
    And I visit "john/cp/content/import?feed_is_imported=All"
    And I should see "JFK was murdered"
    And I click "Dummy feed importer"
    And I should see "JFK was murdered"
    When I visit "john/cp/content/import?feed_by_text=pancakes"
    Then I should not see "JFK was murdered"
