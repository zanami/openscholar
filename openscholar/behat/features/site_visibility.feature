Feature:
  Testing the visibility field.

  @api @current
  Scenario Outline: Define the site visibility field to "Anyone with the link"
                    and test that anonymous users can view the site.
     Given I visit <request-url>
      Then I should see <final-url>

  Examples:
    | request-url                     | final-url           |
    | "einstein"                      | "Einstein"          |
    | "einstein/blog"                 | "Mileva Marić"      |
    | "einstein/blog/mileva-marić"    | "Yesterday i met Mileva, what a nice girl :)."      |
