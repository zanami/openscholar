Feature:
  Verifying a OpenScholar site creation process.

Background: Testing the cretion of a site in OpenScholar
    Given I am logged in as a user with the "administrator" role
    And I visit "/site/register"
    When I fill in "domain" with "foo"
    And I press "Create your site"
    And I visit "/foo"
    And I should see "foo"
    And I should see "Your site's front page is set to display your bio by default."

@api
Scenario: Adding
    Given I am logged in as a user with the "administrator" role
    And I visit "foo/cp/build/features/os_front"
    And I should see "Apps"
    When I select "Public" from "spaces_features[os_blog]"
    And I press "Save for foo"
    When I visit "/foo"
    Then I should see "Blog"
