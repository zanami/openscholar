Feature:
  Testing vsite related permissions.

  @api @me
  Scenario: Test edit ability for a vsite admin on won site.
    Given I am logging in as "alexander"
     When I visit "edison/publications"
     Then I should see the link "Links" under "ctools-dropdown-link-wrapper"


  @api @me
  Scenario: Test edit inability for vsite admin on a different vsite.
    Given I am logging in as "alexander"
     When I visit "john/publications"
     Then I should not see the link "Links" under "ctools-dropdown-link-wrapper"
