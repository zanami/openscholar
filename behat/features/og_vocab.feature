Feature:
  Testing the migrate of OG Vocabulary.

  @api
  Scenario: Test OG Vocabulary migration
    Given I am logged in as a user with the "authenticated user" role
    And I visit "john"
    When I click "Blog"
    Then I should see "Antoine de Saint-Exupéry"
    And I should see "Douglas Noël Adams"
    And I should see "Stephen William Hawking"
