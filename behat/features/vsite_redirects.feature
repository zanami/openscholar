Feature:
  In order to have content at a single URL
  As a visitor
  I want to be redirected to a canonical URL
  
  Background:
    Given a node of type "news" with the title "My News" exists in site "john"
    
  @api
  Scenario Outline: Non-aliased node paths redirect on sites without domains.
    Given I visit <request-url>
     When I should get a <code> HTTP response
     Then I should be on <final-url>

    Examples:
      | request-url         | code | final-url           |
      | "node/123"          | 301  | "john/news/my-news" |
      | "john/node/123"     | 301  | "john/news/my-news" |
      | "jane/node/123"     | 301  | "john/news/my-news" |
      | "john/news/my-news" | 200  | "john/news/my-news" |

  @api
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

  @api
  Scenario Outline: Non-aliased system paths redirect on sites with domains.
    Given I am on a site with a custom domain "custom.com"
     When I visit <request-url>
     Then I should get a <code> HTTP response
      And I should be on <final-url>

    Examples:
      | request-url     | code | final-url |
      | "john"          | 301  | ""        |
      | "john/news"     | 301  | "news"    |