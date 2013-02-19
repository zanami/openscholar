Feature:
  Vsite paths permanently redirect to a single URL.

  @api
  Scenario Outline: Non-aliased node paths redirect on sites without domains.
     When I visit <request-url>
     Then I should get a <code> HTTP response
      And I should end up at <final-url>

    Examples:
      | request-url         | code | final-url           |
      | "node/123"          | 301  | "john/news/my-news" |
      | "john/node/123"     | 301  | "john/news/my-news" |
      | "john/news/my-news" | 200  | "john/news/my-news" |
           
  @api
  Scenario Outline: Non-aliased node paths redirect on sites with domains.
    Given I am on a site with a custom domain
     When I visit <request-url>
     Then I should get a <code> HTTP response
      And I should end up at <final-url>

    Examples:
      | request-url         | code | final-url      |
      | "node/123"          | 301  | "news/my-news" |
      | "john/node/123"     | 301  | "news/my-news" |
      | "john/news/my-news" | 200  | "news/my-news" |

  @api
  Scenario Outline: Non-aliased system paths redirect on sites with domains.
    Given I am on a site with a custom domain
     When I visit <request-url>
     Then I should get a <code> HTTP response
      And I should end up at <final-url>

    Examples:
      | request-url     | code | final-url |
      | "john"          | 301  | ""        |
      | "john/news"     | 301  | "news"    |