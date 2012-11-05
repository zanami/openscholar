Feature:
  Testing the migrations of the nodes.

@api
Scenario: Testing the Bio feature
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    And I am on a "bio" page titled "John doe biography"
    Then I should see "John doe biography"
    And I should see "Work in gizra inc."

@api
Scenario: Testing the News feature
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "News"
    And I should see the page title "John"
    And I should see the link "I opened a new personal"
    When I click "I opened a new personal"
    Then I should see "This is a new site generated via the vsite options in open scholar."

@api
Scenario: Testing the Classes feature
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "Classes"
    And I should see "John F. Kennedy"
    And I should see the link "Wikipedia page on JFK"
    And I should see the page title "John"
    When I click "John F. Kennedy"
    And I should see the link "Who was JFK?"
    When I click "Who was JFK?"
    Then I should see "JFK's childhood"
    And I should see "JFK as president"
    And I should see "After JFK's death, and how it changed USA?"

@api
Scenario: Testing the People feature
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "People"
    And I should see the link "John Fitzgerald Kennedy"
    When I click "John Fitzgerald Kennedy"
    Then I should see the page title "John"

@api
Scenario: Testing the Publication feature
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "Publications"
    And I should see the link "The Little Prince"
    When I click "The Little Prince"
    Then I should see the page title "John"
    And I should see "Anon. The Little Prince. United States; 1943."

@api
Scenario: Check that all of the apps is turned on
  Given I am logged in as a user with the "administrator" role
  And I visit "john"
  When I click "Build"
  Then I should see the page title "Apps"
  And I should see the application table with the following <contents>:
    | Bio/CV        | Public |
    | Blog          | Public |
    | Booklets      | Public |
    | Classes       | Public |
    | Dataverse     | Public |
    | Events        | Public |
    | Image Gallery | Public |
    | Links         | Public |
    | News          | Public |
    | Basic Pages   | Public |
    | Presentations | Public |
    | Profiles      | Public |
    | Publications  | Public |
    | Reader        | Public |
    | Software      | Public |
