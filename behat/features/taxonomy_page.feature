Feature:
  As a user visiting different content-type tabs
  I should be able to filter by terms
  And see nodes of the content-type that are also attached to the selected term.

  @api
  Scenario: Verfity that "classes" tab shows all nodes.
    Given I visit "john/classes"
     Then I should see "John F. Kennedy"
      And I should see "Neil Armstrong"

  @api
  Scenario: Verfity that "classes" tab shows can filter nodes by term.
     Given I visit "john/classes/science/air"
      Then I should see "John F. Kennedy"
       And I should not see "Neil Armstrong"
