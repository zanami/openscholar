Feature:
  I should be able to filter by terms
  And see nodes of the content-type that are also attached to the selected term.

  @api
  Scenario: Verfity that "software" tab shows all nodes.
    Given I visit "john/software"
     Then I should see "Mac OSX"
      And I should see "Windows 7"

  @api
  Scenario: Verfity that "software" tab shows can filter nodes by term.
     Given I visit "john/software/science/air"
      Then I should see "Mac OSX"
       And I should not see "Windows 7"
