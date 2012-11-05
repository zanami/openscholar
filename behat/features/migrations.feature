Feature:
  Testing the migrations of the nodes.

@api
Scenario: Testing the Bio feature
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    And I am on a "bio" page titled "John doe biography"
    And I should see "John doe biography"
    And I should see "Work in gizra inc."

@api
Scenario: Testing the News feature
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "News"
    And I should see the page title "John"
    And I should see the link "I opened a new personal"
    When I click "I opened a new personal"
    And I should see "This is a new site generated via the vsite options in open scholar."

@api
Scenario: Testing the Classes feature
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "Classes"
    And I should see "All about entities and how to use entity metadata wrapper."
    And I should see the link "Drupal entities"
    And I should see the page title "John"
    When I click "Drupal entities"
    And I should see the link "An Introduction to Entities"
    When I click "Drupal entities class material"
    And I should see "Entity field query"
    And I should see "Entity field query VS. DB select"
    And I should see "Entity metadata wrapper"

@api
Scenario: Testing the People feature
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "People"
    And I should see the link "John Fitzgerald Kennedy"
    When I click "John Fitzgerald Kennedy"
    Then I should see the page title "John"
