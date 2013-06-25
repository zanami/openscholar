Feature:
  Testing the term tagged items pager.

  @api
  Scenario: Testing the term tagged items pager.
    Given I visit "/john"
     When I am adding the subtheme "subtheme" in "john"
     When I defined the subtheme "subtheme" of the theme "cleanblue" as the theme of "john"
     Then I should verify the existence of the css "espp.css"
      And I should verify the existence of the js "espp.js"
