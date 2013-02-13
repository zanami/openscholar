Feature:
  Testing the harvard courses import mechanism.

  @javascript @wip
  Scenario: Importing courses and test their grouping to the correct sites.
    Given I am logged in as "admin"

    # Define harvard courses
     When I visit "courses/build-courses-environment"
      And I visit "admin/config/development/performance"
      And I press "Clear all caches"
      And I visit "admin/structure/feeds/course/settings/HarvardFetcher"
      And I check the box "Debug mode"
      And I press "Save"

    # Import harvard courses
      And I visit "john/cp/build/features/harvard_courses"
      And I fill in "Department ID" with "Architecture"
      And I select "Harvard Graduate School of Design" from "School name"
      And I press "Save configuration"
      And I visit "john/courses"
      And I click "Refresh courses"
      And I sleep for "10"
      And I should see "(Re)fabricating Tectonic Prototypes"

    # Remove the courses from the site.
      And I visit "john/cp/build/features/harvard_courses"
      And I press "Remove"
      And I sleep for "2"
      And I press "Save configuration"
      And I visit "john/courses"
      And I should not see "(Re)fabricating Tectonic Prototypes"

    # Re add the courses and verify they were added.
      And I visit "john/cp/build/features/harvard_courses"
      And I fill in "Department ID" with "Architecture"
      And I select "Harvard Graduate School of Design" from "School name"
      And I press "Save configuration"
      And I visit "john/courses"
     Then I should see "(Re)fabricating Tectonic Prototypes"

