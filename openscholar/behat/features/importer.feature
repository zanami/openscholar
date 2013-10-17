Feature: Testing the importer.

  @api
  Scenario Outline: Testing the csv importing for blog.
    Given I am logging in as "admin"
    When I visit <import-address>
    And I visit <address>
    Then I should see <title>
    And I should see <body>

  Examples:
    | import-address                  | address                                   | title                   | body                            |
    | "john/os-importer-demo/blog"    | "john/blog"                               | "Blog from csv"         | "This is a blog from csv file"  |
    | "john/os-importer-demo/news"    | "john/news"                               | "Testing import news"   | "Testing the import of news"    |
    | "john/os-importer-demo/event"   | "john/calendar?type=day&day=2013-10-17"   | "Testing importing"     | ""                              |
