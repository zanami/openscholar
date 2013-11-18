Feature:
  Testing the OS reader functionallity.

  @api
  Scenario: Test the Reader tab
    Given I visit "john"
     When I click "Reader"
     Then I should see "Engadget rss"

  @api
    Scenario: Testing the UI for importing an RSS feed.
      Given I am logging in as "john"
        And I visit "john/cp/os-importer/blog"
        And I select the radio button "Rss" with the id "edit-format-rss"
        And I populate in "Title" with "Import once"
        And I populate in "URL" with "LOCALHOST/os-reader/john_blog"
       When I press "Submit"
        And I visit "john/cp/os-importer/blog/manage"
       Then I should see "Import once"

  @api
    Scenario: Verify the user can't import the same RSS address twice.
      Given I am logging in as "john"
        And I visit "john/cp/os-importer/blog"
        And I select the radio button "Rss" with the id "edit-format-rss"
        And I populate in "Title" with "Import 2nd"
        And I populate in "URL" with "LOCALHOST/os-reader/john_blog"
       When I press "Submit"
        And I visit "john/cp/os-importer/blog/manage"
       Then I should not see "Import 2nd"

  @api
  Scenario: Test the OS reader feed importer.
    Given I am logging in as "admin"
      And I import feed items for "john"
     When I visit "john/cp/os-importer/news/manage"
      And I should see "John news importer"
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
  Scenario: Feed items are displayed for each site
    Given I am logging in as "admin"
      And I import feed items for "obama"
     When I visit "obama/cp/os-importer/news/manage"
      And I should see "Obama news importer"
      And I should not see "John news importer"
      And I should not see "JFK was murdered"
      And I import the feed item "Four more years is the most re-tweeted tweet"
     Then I should see the feed item "Four more years is the most re-tweeted tweet" was imported
      And I should see "Four more years is the most re-tweeted tweet"

  @api
  Scenario: Verify images in feed item description are imported as images.
    Given I am logging in as "admin"
<<<<<<< HEAD
      And I import feed items for "john"
      And I visit "john/cp/os-importer/news/manage"
     When I import the feed item "Lee Harvey Oswald"
      And I should see the feed item "Lee Harvey Oswald" was imported
=======
     When I visit "john/cp/os-importer/news/manage"
      And I should see the feed item "JFK was murdered" was imported
>>>>>>> SCHOLAR-3.x
     Then I should see the news photo "druplicon.small__"

  @api
  Scenario: Test the Contains filter on OS feed import page.
    Given I am logging in as "admin"
      And I visit "john/cp/os-importer/news/manage?feed_by_text=president"
      And I should see "JFK was murdered"
     When I visit "john/cp/os-importer/news/manage?feed_by_text=pancakes"
     Then I should not see "JFK was murdered"

  @api
  Scenario: Test the Status filter on OS feed import page.
    Given I am logging in as "admin"
     When I visit "john/cp/os-importer/news/manage?feed_is_imported=All"
     Then I should see "JFK was murdered"

  @api
  Scenario: Test the Importer filter on OS feed import page.
    Given I am logging in as "admin"
      And I visit "john/cp/os-importer/news/manage"
     When I click "John news importer"
     Then I should see "JFK was murdered"

  @api
  Scenario: Verify the imported news date is the original feed item date.
    Given I visit "john/news"
      And I click "Lee Harvey Oswald"
     Then I should see "November 22, 1963"
