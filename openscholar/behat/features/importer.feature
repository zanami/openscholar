Feature: Testing the importer.

  @api
  Scenario Outline: Testing the csv importing for blog.
    Given I am logging in as "admin"
     When I visit <import-address>
      And I visit <address>
     Then I should see <title>
      And I should see <body>

  Examples:
    | import-address                            | address                                   | title                   | body                            |
    | "john/os-importer-demo/blog"              | "john/blog"                               | "Blog from csv"         | "This is a blog from csv file"  |
    | "john/os-importer-demo/news"              | "john/news"                               | "Testing import news"   | "Testing the import of news"    |
    | "john/os-importer-demo/event"             | "john/calendar?type=day&day=2013-10-17"   | "Testing importing"     | ""                              |
    | "john/os-importer-demo/page"              | "john/page-csv"                           | "Page from csv"         | "This is a page from csv file"  |
    | "john/os-importer-demo/class"             | "john/classes"                            | "Testing importing"     | "Testing importing of class"    |
    | "john/os-importer-demo/faq"               | "john/faq"                                | "FAQ from csv"          | ""                              |
    | "john/os-importer-demo/presentation"      | "john/presentations"                      | "Presentation from csv" | ""                              |
    | "john/os-importer-demo/software_project"  | "john/software"                           | "Software from csv"     | "Testing import software"       |
    | "john/os-importer-demo/link"              | "john/links"                              | "Link from csv"         | "Testing import link"           |
    | "john/os-importer-demo/person"            | "john/people"                             | "Homer J Simpson"       | "Doh!"                          |
    | "john/os-importer-demo/media_gallery"     | "john/galleries"                          | "Gallery from csv"      | "Testing import gallery"        |

  @api
    Scenario: Verify that the vocabularies and terms from the CSV created
              successfully.
      Given I visit "john/blog/blog-csv"
       Then I should see "Johnny B good"
        And I should see "Californication"
        And I should see "Chuck Berry"
        And I should see "Red hot chili peppers"
