Feature: Testing the importer.

  @api @wip
  Scenario: Testing the csv importing.
    Given I am logging in as "admin"
     When I visit "john/os-importer/os_blog_csv"
      And I import the "blog" csv file
