Feature:
  Verifying a OpenScholar site creation process.

@api
Scenario: Testing the cretion of a site in OpenScholar
    Given I am logged in as a user with the "administrator" role
    And I visit "/site/register"
    When I fill in "domain" with "foo"
    And I press "Create your site"
    And I visit "/foo"
    And I should see "foo"
    And I should see "Your site's front page is set to display your bio by default."
