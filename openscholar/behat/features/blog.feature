Feature:
  Testing the blog tab.

  @api
  Scenario: Test the Blog tab
     Given I visit "john"
      When I click "Blog"
      Then I should see "First blog"

  @api
  Scenario: Test the Blog archive
    Given I visit "john"
    And I click "Blog"
    And I should see "ARCHIVE"
    When I visit "john/blog/archive/all"
    And I should see "First blog"
    And I visit "john/blog/archive/all/201301"
    Then I should see "Archive: January 2013"
    And I should not see "First blog"

  @api
  Scenario: Test oembed
    Given I am logging in as "john"
    And I visit "john/node/add/blog"
    And I fill in "Title" with "embed test"
    And I fill in "Body" with "<p>[[{\"fid\":\"10\",\"view_mode\":\"default\",\"type\":\"media\",\"attributes\":{\"height\":292,\"width\":389,\"title\":\"Kate Stone: DJ decks made of... paper | Video on TED.com\",\"class\":\"media-element file-default\"}}]]</p>"
    And I press "Save"
    When I visit "john/blog/embed-test"
    Then I should see the embed "http://embed.ted.com/talks/kate_stone_dj_decks_made_of_paper.html"
