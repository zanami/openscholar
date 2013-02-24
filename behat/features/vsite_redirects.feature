Feature:
  In order to have content at a single URL as a visitor i want to be redirected
  to a canonical URL.

  @api
  Scenario: Non-aliased node paths redirect on sites without domains.
    Given I expect for a behavior according the next <statements>:
      | john          | 200  | john                       |
      | node/1        | 302  | john                       |
      | node/25       | 302  | john/book/all-about-nodes  |
      | john/node/25  | 302  | john/book/all-about-nodes  |
      | als/node/25   | 302  | john/book/all-about-nodes  |

  @api @wip
  Scenario Outline: Non-aliased node paths redirect on sites with domains.
    Given I am on a site with a custom domain "custom.com"
     When I visit <request-url>
     Then I should get a <code> HTTP response
      And I should be on <final-url>

    Examples:
      | request-url         | code | final-url      |
      | "node/123"          | 301  | "news/my-news" |
      | "john/node/123"     | 301  | "news/my-news" |
      | "john/news/my-news" | 200  | "news/my-news" |

  @api @wip
  Scenario Outline: Non-aliased system paths redirect on sites with domains.
    Given I am on a site with a custom domain "custom.com"
     When I visit <request-url>
     Then I should get a <code> HTTP response
      And I should be on <final-url>

    Examples:
      | request-url     | code | final-url |
      | "john"          | 301  | ""        |
      | "john/news"     | 301  | "news"    |