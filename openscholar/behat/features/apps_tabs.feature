Feature:
  Testing tabs of all the enabled Apps appear, and there is content in
  them.

  @api @debug
  Scenario: Debug
    Given I visit "/"
    Then I should print page

  @api
  Scenario: Test the Bio tab
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "Bio"
    And I click "John doe biography"
    Then I should see "Work in gizra inc."

  @api
  Scenario: Test the News tab
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "News"
    And I click "I opened a new personal"
    Then I should see "This is a new site generated via the vsite options in open scholar."

  @api
  Scenario: Test the Classes tab
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "Classes"
    And I click "John F. Kennedy"
    Then I should see the link "Wikipedia page on JFK"
    And I should see the link "Who was JFK?"

  @api
  Scenario: Test the People tab
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "People"
    When I click "John Fitzgerald Kennedy"
    Then I should see "often referred to by his initials JFK"

  @api
  Scenario: Test the Publication tab
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "Publications"
    And I click "The Little Prince"
    Then I should see "Anon. The Little Prince. United States; 1943."
