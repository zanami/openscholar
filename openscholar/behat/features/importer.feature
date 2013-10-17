Feature: Testing the importer.

  @api
  Scenario: Testing the csv importing.
    Given I am logging in as "admin"
     When I visit "john/os-importer-demo/blog"
      And I visit "john/blog"
     Then I should see "Blog from csv"
      And I should see "This is a blog from csv file"
