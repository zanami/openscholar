Feature:
  Testing the news tab.

  @api
  Scenario: Test the News tab
    Given I visit "john"
     When I click "News"
      And I click "I opened a new personal"
     Then I should see "This is a new site generated via the vsite options in open scholar."


  @api
  Scenario: Verfity that "news" tab shows all nodes.
    Given I visit "john/news"
     Then I should see "I opened a new personal"
      And I should see "More tests to the semester"

  @api
  Scenario: Verfity that "news" tab shows can filter nodes by term.
     Given I visit "john/news/science/air"
      Then I should see "I opened a new personal"
       And I should not see "More tests to the semester" under "content"
