Feature:
  Testing the filter by term widget.

  @api @current
  Scenario: Verfity that the user sees terms in the filter by term widget.
    Given the widget "Filter by term" is set in the "Publications" page
     When I visit "john/publications"
     Then I should see "Filter by term"
