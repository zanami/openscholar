Feature:
  Testing the harvard courses import mechanism.

  @api @wip
  Scenario: Importing courses and test their grouping to the correct sites.
    Given I am logged in as "admin"
     When I visit "courses/build-courses-environment"
      And I visit "john/cp/build/features/harvard_courses"
      And I fill in "Department ID" with "Architecture"
      And I select "Harvard Graduate School of Design" from "School name"
      And I press "Save configuration"
      And I visit "courses/refresh-courses"
      And I sleep for "10"
      And I visit "john/courses"
      And I should see "(Re)fabricating Tectonic Prototypes"
      And I visit "john/cp/build/features/harvard_courses"
      And I press "Remove"
      And I sleep for "2"
      And I press "Save configuration"
      And I visit "john/courses"
      And I should not see "(Re)fabricating Tectonic Prototypes"
      And I visit "john/cp/build/features/harvard_courses"
      And I fill in "Department ID" with "Architecture"
      And I select "Harvard Graduate School of Design" from "School name"
      And I press "Save configuration"
      And I visit "john/courses"
     Then I should see "(Re)fabricating Tectonic Prototypes"

