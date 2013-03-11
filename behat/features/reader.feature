Feature:
  Testing the reader tab.

  @api
  Scenario: Test the Reader tab
    Given I visit "john"
     When I click "Reader"
     Then I should see "Engadget rss"

  @api
  Scenario: Verfity that "reader" tab shows all nodes.
    Given I visit "john/reader"
     Then I should see "Engadget rss"
      And I should see "Feeds around the world"

  @api
  Scenario: Verfity that "reader" tab shows can filter nodes by term.
     Given I visit "john/reader/science/air"
      Then I should see "Engadget rss"
       And I should not see "Feeds around the world"

