Feature:
  Testing the filter by term widget.

  @api
  Scenario: Verfity that the user sees terms in the filter by term widget.
     Given I am logged in as a user with the "administrator" role
       And the widget "Filter by term" is set in the "Publications" page with the following <settings>:
           | Vocabularies     | authors | select list |
           | Show empty terms | check   | checkbox    |
      When I visit "john/publications"
      Then I should see "Filter by term"
       And I should see the following <links>
           | Antoine de Saint-Exupéry |
           | Douglas Noël Adams       |
           | Antoine de Saint-Exupéry |

  @api
  Scenario: Verfity that the user sees terms in the filter by term widget.
     Given I am logged in as a user with the "administrator" role
       And the widget "Filter by term" is set in the "Classes" page with the following <settings>:
           | Vocabularies     | science | select list |
       And I set the term "Fire" under the term "Air"
      When I visit "john/classes"
      Then I check the "Air" term link to the original page
       And I check the "Fire" term not link to the original page
