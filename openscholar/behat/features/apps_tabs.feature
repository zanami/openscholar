Feature:
  Testing tabs of all the enabled Apps appear, and there is content in
  them.

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

  @api
  Scenario: Test the Calendar tab
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "Calendar"
    Then I should see "John F. Kennedy birthday"

  @api
  Scenario: Test the Blog tab
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "Blog"
    Then I should see "First blog"

  @api
  Scenario: Test the Persentation tab
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "Presentations"
    Then I should see "JFK's biography"

  @api
  Scenario: Test the Links tab
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "Links"
    Then I should see "JFK wikipedia page"

  @api
  Scenario: Test the Documents tab
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "Documents"
    Then I should see "All about nodes"

  @api
  Scenario: Test the Reader tab
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "Reader"
    Then I should see "Engadget rss"

  @api
  Scenario: Test the Galleries tab
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "Galleries"
    Then I should see the images:
      | slideshow1.jpg |
      | slideshow2.jpg |
      | slideshow3.jpg |
