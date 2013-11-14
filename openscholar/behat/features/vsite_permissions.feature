Feature:
  Testing vsite related permissions.

  @api @current
  Scenario: Testing normal user can't access the control panel
    Given I am logging in as "demo"
     When I go to "cp/build/features"
     Then I should get a "403" HTTP response

  @api @current
  Scenario: Testing normal user can't access the control panel
    Given I am logging in as "michelle"
     When I go to "node/add"
     Then I should get a "200" HTTP response

  @api @current
  Scenario: Testing normal user can't access the control panel
    Given I am logging in as "michelle"
     When I go to "john/blog/unpublish-blog"
     Then I should get a "403" HTTP response

  @api @current
  Scenario: Testing normal user can't access the control panel
    Given I am logging in as "john"
     When I go to "john/blog/unpublish-blog"
     Then I should get a "200" HTTP response

  @api @current
  Scenario: Testing normal user can't access the control panel
    Given I am logging in as "michelle"
     When I go to "obama/node/add/blog"
      And I should get a "200" HTTP response
      And I go to "john/node/add/blog"
     Then I should get a "403" HTTP response

  @api @current
  Scenario: Test edit ability for a vsite admin on won site.
    Given I am logging in as "alexander"
     When I visit "edison/publications"
     Then I should see the link "Links" under "ctools-dropdown-link-wrapper"

  @api @current
  Scenario: Test edit inability for vsite admin on a different vsite.
    Given I am logging in as "alexander"
     When I visit "john/publications"
     Then I should not see the link "Links" under "ctools-dropdown-link-wrapper"
