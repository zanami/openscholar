Feature:
  In order to have content at a single URL as a visitor i want to be redirected
  to a canonical URL.

  @api
  Scenario: Non-aliased node paths redirect on sites without domains.
    Given I expect for a behavior according the next <statements>:
   #  | Address                                 | Code  | Expected URL                              |
      | john                                    | 200   | john                                      |
      | node/1                                  | 302   | john                                      |
      | book/all-about-nodes                    | 302   | john/book/all-about-nodes                 |
      | als/book/all-about-nodes                | 302   | john/book/all-about-nodes                 |
      | john/classes/all-about-abraham-lincoln  | 302   | lincoln/classes/all-about-abraham-lincoln |

  @api
  Scenario: Non-aliased node paths redirect on sites with domains.
    Given I expect for a behavior according the next <statements>:
    # |   Prefix  | Title                   | Path? | Code  | Expected URL                        |
      |           | John Fitzgerald Kennedy | No    | 302   | john/people/john-fitzgerald-kennedy |
      | john/     | John Fitzgerald Kennedy | No    | 302   | john/people/john-fitzgerald-kennedy |
      | john/     | John Fitzgerald Kennedy | Yes   | 200   | john/people/john-fitzgerald-kennedy |

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